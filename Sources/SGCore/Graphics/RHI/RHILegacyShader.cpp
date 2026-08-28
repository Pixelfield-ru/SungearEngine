//
// Created by stuka on 07.07.2023.
//

#include "RHILegacyShader.h"

#include <charconv>
#include <cstring>
#include <set>
#include <glm/gtc/type_ptr.hpp>

#include "IDevice.h"
#include "LiveDevice.h"
#include "SGCore/Graphics/RHI/SharedUniformBuffers.h"
#include "SGCore/Graphics/RHI/TextureUnits.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/RHI/RHIShaderLoader.h"
#include "SGCore/Graphics/SPIRV/SPIRVCompiler.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Utils/SGSL/SGSLEVulkanizer.h"
#include "SGCore/Utils/Utils.h"

namespace
{
    SGCore::IDevice* currentDevice() noexcept
    {
        // never through the renderer singleton: assets are destroyed in static destruction, when it
        // may already be gone (see docs/RHI_DESIGN.md and the sgcore-graphics skill)
        return SGCore::LiveDevice::get();
    }

    /// Same initializer subset the shader corpus uses (numeric literals, vecN constructors) as
    /// GL46Shader evaluates: uniforms that carried an initializer lose it when moved into a block.
    bool evaluateInitializer(std::string_view expression, std::vector<float>& out) noexcept
    {
        out.clear();

        auto parseNumber = [](std::string_view text, float& value)
        {
            while(!text.empty() && (text.front() == ' ' || text.front() == '\t')) text.remove_prefix(1);
            while(!text.empty() && (text.back() == ' ' || text.back() == '\t' || text.back() == 'f' || text.back() == 'F')) text.remove_suffix(1);
            if(text == "true") { value = 1.0f; return true; }
            if(text == "false") { value = 0.0f; return true; }
            const auto result = std::from_chars(text.data(), text.data() + text.size(), value);
            return result.ec == std::errc { } && result.ptr == text.data() + text.size();
        };

        const auto open = expression.find('(');
        if(open == std::string_view::npos)
        {
            float value = 0.0f;
            if(!parseNumber(expression, value)) return false;
            out.push_back(value);
            return true;
        }

        const auto typeName = expression.substr(0, open);
        std::size_t components = 0;
        if(typeName.find("vec4") != std::string_view::npos) components = 4;
        else if(typeName.find("vec3") != std::string_view::npos) components = 3;
        else if(typeName.find("vec2") != std::string_view::npos) components = 2;
        else return false;

        const auto close = expression.rfind(')');
        if(close == std::string_view::npos || close <= open) return false;
        const std::string_view arguments = expression.substr(open + 1, close - open - 1);

        std::vector<float> values;
        std::size_t start = 0;
        while(start <= arguments.size())
        {
            const auto comma = arguments.find(',', start);
            const auto piece = arguments.substr(start, comma == std::string_view::npos ? std::string_view::npos : comma - start);
            float value = 0.0f;
            if(!parseNumber(piece, value)) return false;
            values.push_back(value);
            if(comma == std::string_view::npos) break;
            start = comma + 1;
        }

        if(values.size() == 1) out.assign(components, values.front()); // vecN(x) broadcasts
        else if(values.size() == components) out = values;
        else return false;

        return true;
    }
}

SGCore::RHILegacyShader::~RHILegacyShader() noexcept
{
    RHILegacyShader::destroy();
}

void SGCore::RHILegacyShader::doCompile()
{
    destroyLegacyBlocks();
    m_reflection = ShaderReflection { };
    m_rhiProgram = nullptr;
    m_samplerUnits.clear();
    m_descriptorSet = nullptr;

    auto* device = currentDevice();
    if(!device) return;

    const auto shaderAnalyzedFile = getAnalyzedFile();
    if(!shaderAnalyzedFile || shaderAnalyzedFile->getSubShaders().empty())
    {
        SG_LOG_E("RHILegacyShader '{}': no stages after translation.", Utils::toUTF8(getPath().raw()));
        return;
    }

    // explicit APIs consume the GLSL4 variant through the vulkanizer (same choice RHIShaderLoader makes)
    std::string definesCode = "#define SG_GLSL4 \n";
    for(const auto& [defineType, defines] : getDefines())
    {
        for(const auto& define : defines) definesCode += define.toString() + "\n";
    }
    for(const auto& [name, value] : shaderAnalyzedFile->getAttributes())
    {
        definesCode += "#define " + name + " " + value + "\n";
    }

    std::vector<SGSLEVulkanizer::Stage> stages;
    for(const auto& subShader : shaderAnalyzedFile->getSubShaders())
    {
        stages.push_back({ subShader.getType(), definesCode + "\n" + subShader.getCode() });
    }

    SGSLEVulkanizer::Config config;
    config.m_target = SGSLEVulkanizer::Target::VULKAN;
    const auto vulkanizeReport = SGSLEVulkanizer::vulkanize(stages, config);
    for(const auto& warning : vulkanizeReport.m_warnings)
    {
        SG_LOG_W("RHILegacyShader '{}': {}", Utils::toUTF8(getPath().raw()), warning);
    }

    ShaderProgramDesc programDesc;
    programDesc.m_debugName = Utils::toUTF8(getPath().resolved().filename());
    for(const auto& stage : stages) programDesc.m_stages.push_back({ stage.m_type, stage.m_code, { } });

    // the backend program compiles the SPIR-V itself when a stage carries none
    m_rhiProgram = device->createShaderProgram(programDesc);
    if(!m_rhiProgram || !m_rhiProgram->isValid())
    {
        SG_LOG_E("RHILegacyShader '{}' failed to compile:\n{}", Utils::toUTF8(getPath().raw()),
                 m_rhiProgram ? m_rhiProgram->getLog() : "createShaderProgram returned null");
        m_rhiProgram = nullptr;
        return;
    }

    m_reflection = m_rhiProgram->getReflection();
    setupLegacyBlocks();

    std::vector<float> values;
    for(const auto& defaultValue : vulkanizeReport.m_defaults)
    {
        if(!evaluateInitializer(defaultValue.m_expression, values))
        {
            SG_LOG_W("RHILegacyShader '{}': can not evaluate initializer '{}' of uniform '{}'; it stays zero.",
                     Utils::toUTF8(getPath().raw()), defaultValue.m_expression, defaultValue.m_memberName);
            continue;
        }
        writeLegacy(defaultValue.m_memberName, values.data(), static_cast<std::uint32_t>(values.size() * sizeof(float)));
    }
}

void SGCore::RHILegacyShader::setupLegacyBlocks() noexcept
{
    auto* device = currentDevice();
    if(!device) return;

    m_descriptorSet = device->createDescriptorSet();

    for(const auto& binding : m_reflection.m_bindings)
    {
        if(binding.m_type != ShaderDescriptorType::UNIFORM_BUFFER) continue;
        if(binding.m_name.rfind("SGLegacyUniforms", 0) != 0) continue;

        LegacyBlock block;
        block.m_binding = binding.m_binding;
        block.m_size = binding.m_blockSize;
        // zero-initialized: uniforms nobody sets read as 0 / false, like a fresh GL program
        block.m_values.assign(binding.m_blockSize, std::uint8_t { 0 });

        const std::size_t blockIndex = m_legacyBlocks.size();
        m_legacyBlocks.push_back(std::move(block));

        for(const auto& member : binding.m_members)
        {
            LegacyMemberRef reference;
            reference.m_blockIndex = blockIndex;
            reference.m_offset = member.m_offset;
            reference.m_size = member.m_size;
            reference.m_arrayCount = member.m_arrayCount;
            reference.m_elementStride = member.m_arrayCount > 1 ? member.m_paddedSize : 0;
            m_legacyMembers.emplace(member.m_name, std::vector<LegacyMemberRef> { }).first->second.push_back(reference);
        }
    }
}

void SGCore::RHILegacyShader::destroyLegacyBlocks() noexcept
{
    // the GPU copies live in the device's uniform arena, which outlives every shader
    m_legacyBlocks.clear();
    m_legacyMembers.clear();
}

void SGCore::RHILegacyShader::destroy() noexcept
{
    destroyLegacyBlocks();
    m_samplerUnits.clear();
    m_descriptorSet = nullptr;

    if(m_rhiProgram)
    {
        if(auto* device = currentDevice()) device->destroyDeferred(m_rhiProgram);
        m_rhiProgram = nullptr;
    }
    m_reflection = ShaderReflection { };
}

void SGCore::RHILegacyShader::bind() const noexcept
{
    // no "current program" on an explicit API: the pipeline carries the program, and the renderer
    // picks it up from the shader the pass bound last (the trick GL46Shader uses for renderMeshData)
    setAsCurrentShader();
}

const SGCore::Ref<SGCore::IDescriptorSet>& SGCore::RHILegacyShader::buildDescriptorSet() noexcept
{
    if(!m_descriptorSet) return m_descriptorSet;

    // This runs once per draw, and the values the passes set before it are this draw's alone: on GL
    // they went straight into the program, while here the draw is only recorded and runs later. So the
    // block is copied into a slice of its own, which nothing after this draw writes to. Sharing one
    // buffer instead makes every draw of a pass read the values of the pass's last draw — the scene
    // then collapses onto the last object's transform and material.
    if(auto* device = currentDevice())
    {
        for(auto& block : m_legacyBlocks)
        {
            const auto slice = device->allocateTransientUniforms(block.m_size);
            if(!slice.m_mapped) continue;

            std::memcpy(slice.m_mapped, block.m_values.data(), block.m_values.size());
            m_descriptorSet->setUniformBuffer(block.m_binding, slice.m_buffer, slice.m_offset, block.m_size);
        }
    }

    // the engine's shared blocks (CameraData, ProgramDataBlock, ...) are declared without an explicit
    // binding, so they are matched by block name, not by IUniformBuffer::setLayoutLocation
    for(const auto& binding : m_reflection.m_bindings)
    {
        if(binding.m_type != ShaderDescriptorType::UNIFORM_BUFFER) continue;
        if(binding.m_name.rfind("SGLegacyUniforms", 0) == 0) continue; // this shader's own block

        const auto& shared = SharedUniformBuffers::get(binding.m_name);
        if(shared) m_descriptorSet->setUniformBuffer(binding.m_binding, shared);
        {
            static std::set<std::string> s_reported;
            if(s_reported.insert(binding.m_name).second)
            {
            }
        }
    }

    // descriptors only one backend knows about (Vulkan: the dummy texel buffer every declared
    // samplerBuffer needs, or the draw is dropped outright)
    fillBackendDescriptors(*m_descriptorSet);

    // join the two halves of the unit model: sampler name -> unit (recorded here) -> texture (TextureUnits)
    for(const auto& binding : m_reflection.m_bindings)
    {
        // UNIFORM_TEXEL_BUFFER belongs here too: a GLSL samplerBuffer is addressed by the same unit
        // model (useTextureBlock + ITexture2D::bind of an SG_TEXTURE_BUFFER texture), it just ends up
        // as a buffer view rather than an image. Leaving it out meant every samplerBuffer kept the
        // dummy view fillBackendDescriptors had put there, and batching read nothing.
        if(binding.m_type != ShaderDescriptorType::COMBINED_IMAGE_SAMPLER &&
           binding.m_type != ShaderDescriptorType::SAMPLED_IMAGE &&
           binding.m_type != ShaderDescriptorType::UNIFORM_TEXEL_BUFFER) continue;

        // an array sampler is addressed element-wise by the passes ("mat_diffuseSamplers[0]"), while
        // reflection reports one binding under the base name — resolve every element
        const std::uint32_t count = binding.m_count == 0 ? 1 : binding.m_count;
        std::set<std::uint32_t> boundElements;
        for(std::uint32_t element = 0; element < count; ++element)
        {
            auto unitIt = m_samplerUnits.find(binding.m_name + "[" + std::to_string(element) + "]");
            if(unitIt == m_samplerUnits.end() && element == 0) unitIt = m_samplerUnits.find(binding.m_name);
            if(unitIt == m_samplerUnits.end()) continue;

            // the unit table is backend-neutral: what is stored there is this backend's own texture
            const auto& texture = TextureUnits::get(unitIt->second);
            if(!texture) continue;

            m_descriptorSet->setBackendTexture(binding.m_binding, texture, element);
            boundElements.insert(element);
        }

        // An explicit API refuses (or silently drops) a draw whose declared descriptor was never
        // written, while the passes leave a sampler unbound whenever the material has no texture of
        // that slot (harmless on GL, where it just reads unit 0). Fill the rest with 1x1 white.
        for(std::uint32_t element = 0; element < count; ++element)
        {
            if(boundElements.contains(element)) continue;
            // a texel buffer takes a buffer view, not an image: its fallback is the dummy view that
            // fillBackendDescriptors already wrote, and a 1x1 white image would be the wrong kind
            if(binding.m_type == ShaderDescriptorType::UNIFORM_TEXEL_BUFFER) continue;
            if(auto* device = currentDevice())
            {
                if(const auto dummy = device->getDummyBackendTexture())
                {
                    m_descriptorSet->setBackendTexture(binding.m_binding, dummy, element);
                }
            }
        }
    }

    return m_descriptorSet;
}

bool SGCore::RHILegacyShader::writeLegacy(std::string_view uniformName, const void* data, std::uint32_t size) noexcept
{
    if(m_legacyMembers.empty()) return false;

    // "name[i]" → base member + i * element stride
    std::uint32_t index = 0;
    bool indexed = false;
    std::string_view base = uniformName;
    if(const auto bracket = uniformName.find('['); bracket != std::string_view::npos)
    {
        base = uniformName.substr(0, bracket);
        const auto close = uniformName.find(']', bracket);
        const auto digits = uniformName.substr(bracket + 1, close == std::string_view::npos ? std::string_view::npos : close - bracket - 1);
        const auto result = std::from_chars(digits.data(), digits.data() + digits.size(), index);
        if(result.ec != std::errc { }) return false;
        indexed = true;
    }

    const auto it = m_legacyMembers.find(std::string(base));
    if(it == m_legacyMembers.end()) return false;

    // a uniform declared in several stages is a member of each stage's block: write all of them
    for(const auto& member : it->second)
    {
        auto& block = m_legacyBlocks[member.m_blockIndex];
        std::uint32_t offset = member.m_offset;
        std::uint32_t capacity = member.m_size;
        if(indexed)
        {
            if(index >= member.m_arrayCount) continue;
            offset = member.m_offset + index * member.m_elementStride;
            capacity = member.m_elementStride;
        }
        // std140 padding makes capacity ≥ the value size for scalars/vectors/matrices; never overrun
        const std::uint32_t bytes = capacity == 0 || size < capacity ? size : capacity;
        if(offset + bytes > block.m_values.size()) continue;
        std::memcpy(block.m_values.data() + offset, data, bytes);
    }
    return true;
}

bool SGCore::RHILegacyShader::isLegacyMember(std::string_view uniformName) const noexcept
{
    const auto bracket = uniformName.find('[');
    return m_legacyMembers.contains(std::string(bracket == std::string_view::npos ? uniformName : uniformName.substr(0, bracket)));
}

std::int32_t SGCore::RHILegacyShader::getShaderUniformLocation(const std::string& uniformName) noexcept
{
    // uniform locations do not exist here; everything goes through blocks and descriptor sets
    return isLegacyMember(uniformName) ? 0 : -1;
}

void SGCore::RHILegacyShader::useUniformBuffer(const Ref<IUniformBuffer>&)
{
    // legacy IUniformBuffer objects are a GL concept (glUniformBlockBinding); here the
    // vulkanizer already gave every block an explicit binding
}

void SGCore::RHILegacyShader::useTexture(const std::string& uniformName, const std::uint8_t& texBlock)
{
    m_samplerUnits[uniformName] = texBlock;
}

void SGCore::RHILegacyShader::useTextureBlock(const std::string& uniformName, const int& textureBlock)
{
    if(textureBlock < 0 || textureBlock >= TextureUnits::max_units) return;
    m_samplerUnits[uniformName] = static_cast<std::uint8_t>(textureBlock);
}

void SGCore::RHILegacyShader::useMatrix(const std::string& uniformName, const glm::mat4& matrix)
{
    writeLegacy(uniformName, glm::value_ptr(matrix), sizeof(glm::mat4));
}

void SGCore::RHILegacyShader::useVectorf(const std::string& uniformName, const float& x, const float& y)
{
    const float values[2] = { x, y };
    writeLegacy(uniformName, values, sizeof(values));
}

void SGCore::RHILegacyShader::useVectorf(const std::string& uniformName, const float& x, const float& y, const float& z)
{
    const float values[3] = { x, y, z };
    writeLegacy(uniformName, values, sizeof(values));
}

void SGCore::RHILegacyShader::useVectorf(const std::string& uniformName, const float& x, const float& y, const float& z, const float& w)
{
    const float values[4] = { x, y, z, w };
    writeLegacy(uniformName, values, sizeof(values));
}

void SGCore::RHILegacyShader::useVectorf(const std::string& uniformName, const glm::vec2& vec)
{
    writeLegacy(uniformName, glm::value_ptr(vec), sizeof(glm::vec2));
}

void SGCore::RHILegacyShader::useVectorf(const std::string& uniformName, const glm::vec3& vec)
{
    writeLegacy(uniformName, glm::value_ptr(vec), sizeof(glm::vec3));
}

void SGCore::RHILegacyShader::useVectorf(const std::string& uniformName, const glm::vec4& vec)
{
    writeLegacy(uniformName, glm::value_ptr(vec), sizeof(glm::vec4));
}

void SGCore::RHILegacyShader::useFloat(const std::string& uniformName, const float& f)
{
    writeLegacy(uniformName, &f, sizeof(float));
}

void SGCore::RHILegacyShader::useInteger(const std::string& uniformName, const int& i)
{
    if(writeLegacy(uniformName, &i, sizeof(int))) return;

    // On GL a sampler is an int uniform and glUniform1i on it selects the texture unit, so passes set
    // samplers both ways: useTextureBlock() and useInteger() (PostProcessPass does the latter for an
    // effect's own attachments). Route the second form to the same unit table, otherwise the sampler
    // stays unbound and reads the dummy white texture — which is what turned the SSAO composite white.
    if(isSamplerBinding(uniformName)) useTextureBlock(uniformName, i);
}

bool SGCore::RHILegacyShader::isSamplerBinding(const std::string& uniformName) const noexcept
{
    const auto bracket = uniformName.find('[');
    const auto* binding = m_reflection.findBinding(bracket == std::string::npos ? std::string_view(uniformName)
                                                                                : std::string_view(uniformName).substr(0, bracket));
    if(!binding) return false;
    return binding->m_type == ShaderDescriptorType::COMBINED_IMAGE_SAMPLER ||
           binding->m_type == ShaderDescriptorType::SAMPLED_IMAGE ||
           binding->m_type == ShaderDescriptorType::UNIFORM_TEXEL_BUFFER;
}

bool SGCore::RHILegacyShader::isUniformExists(const std::string& uniformName) const noexcept
{
    if(isLegacyMember(uniformName)) return true;

    // reflection names an array sampler without the index ("mat_diffuseSamplers"), while callers ask
    // for elements: IShader::bindMaterialTextures walks "name[i]" and STOPS at the first name that
    // does not exist, so answering false here silently unbinds every material texture
    const auto bracket = uniformName.find('[');
    if(bracket == std::string::npos) return m_reflection.findBinding(uniformName) != nullptr;

    const auto* binding = m_reflection.findBinding(std::string_view(uniformName).substr(0, bracket));
    if(!binding) return false;

    std::uint32_t index = 0;
    const auto close = uniformName.find(']', bracket);
    const auto digits = std::string_view(uniformName).substr(bracket + 1, close == std::string::npos ? std::string_view::npos : close - bracket - 1);
    if(std::from_chars(digits.data(), digits.data() + digits.size(), index).ec != std::errc { }) return false;

    return index < (binding->m_count == 0 ? 1 : binding->m_count);
}

void SGCore::RHILegacyShader::useMaterialFactors(const SGCore::IMaterial* material)
{
    useVectorf("u_materialDiffuseCol", material->getDiffuseColor());
    useVectorf("u_materialSpecularCol", material->getSpecularColor());
    useVectorf("u_materialAmbientCol", material->getAmbientColor());
    useVectorf("u_materialEmissionCol", material->getEmissionColor());
    useVectorf("u_materialTransparentCol", material->getTransparentColor());
    useFloat("u_materialShininess", material->getShininess());
    useFloat("u_materialMetallicFactor", material->getMetallicFactor());
    useFloat("u_materialRoughnessFactor", material->getRoughnessFactor());
}
