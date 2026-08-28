//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_RHILEGACYSHADER_H
#define SUNGEARENGINE_RHILEGACYSHADER_H

#include <string_view>
#include <unordered_map>
#include <vector>

#include <sgcore_export.h>

#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/RHI/IDescriptorSet.h"
#include "SGCore/Graphics/RHI/IGPUBuffer.h"
#include "SGCore/Graphics/RHI/IShaderProgram.h"
#include "SGCore/Graphics/SPIRV/ShaderReflection.h"

namespace SGCore
{
    /// Legacy IShader facade over an RHI shader program, shared by every explicit backend and the
    /// counterpart of GL46Shader. The backend-specific half is two hooks at the bottom.
    ///
    /// Uniforms: the vulkanizer moves loose uniforms of every stage into `SGLegacyUniforms_<stage>`
    /// std140 blocks; this class keeps a CPU copy per block and routes `useX("name", …)` into it by
    /// offset/stride from reflection — render passes keep calling the same methods.
    ///
    /// Samplers: `useTextureBlock(name, unit)` only records "sampler name reads unit U"; the
    /// unit->texture half lives in TextureUnits. Both halves are joined into an IDescriptorSet at
    /// draw time (buildDescriptorSet), because an explicit API has no unit model.
    class SGCORE_EXPORT RHILegacyShader : public IShader
    {
    public:
        ~RHILegacyShader() noexcept override;

        void destroy() noexcept final;

        void bind() const noexcept final;

        void useUniformBuffer(const Ref<IUniformBuffer>&) override;

        std::int32_t getShaderUniformLocation(const std::string& uniformName) noexcept final;

        void useTexture(const std::string& uniformName, const std::uint8_t& texBlock) final;
        void useMatrix(const std::string& uniformName, const glm::mat4& matrix) final;
        void useVectorf(const std::string& uniformName, const float& x, const float& y) override;
        void useVectorf(const std::string& uniformName, const float& x, const float& y, const float& z) override;
        void useVectorf(const std::string& uniformName, const float& x, const float& y, const float& z, const float& w) override;
        void useVectorf(const std::string& uniformName, const glm::vec2& vec) override;
        void useVectorf(const std::string& uniformName, const glm::vec3& vec) override;
        void useVectorf(const std::string& uniformName, const glm::vec4& vec) override;
        void useFloat(const std::string& uniformName, const float& f) override;
        void useInteger(const std::string& uniformName, const int& i) override;
        void useTextureBlock(const std::string& uniformName, const int& textureBlock) override;
        void useMaterialFactors(const IMaterial* material) override;
        [[nodiscard]] bool isUniformExists(const std::string& uniformName) const noexcept override;

        [[nodiscard]] const ShaderReflection& getReflection() const noexcept { return m_reflection; }
        [[nodiscard]] const Ref<IShaderProgram>& getRHIProgram() const noexcept { return m_rhiProgram; }
        [[nodiscard]] bool isValid() const noexcept { return m_rhiProgram && m_rhiProgram->isValid(); }

        /// Resolves the recorded sampler units against TextureUnits and returns the set to bind for
        /// the next draw. Called by the renderer right before recording a draw.
        [[nodiscard]] const Ref<IDescriptorSet>& buildDescriptorSet() noexcept;

    protected:
        RHILegacyShader() noexcept = default;

        /// Tells the renderer that this shader is the one the passes bound last: an explicit API has
        /// no "current program", so draws take the program from there (as GL46Shader does).
        virtual void setAsCurrentShader() const noexcept = 0;
        /// Backend-specific descriptors the shared path knows nothing about (Vulkan: the dummy texel
        /// buffer every declared samplerBuffer needs).
        virtual void fillBackendDescriptors(IDescriptorSet& /*set*/) noexcept { }

    private:
        struct LegacyBlock
        {
            std::uint32_t m_binding { };
            std::uint32_t m_size { };
            /// The values the passes have set so far. Kept on the CPU because the GPU copy a draw reads
            /// has to be taken per draw: see RHILegacyShader::buildDescriptorSet().
            std::vector<std::uint8_t> m_values;
        };

        struct LegacyMemberRef
        {
            std::size_t m_blockIndex { };
            std::uint32_t m_offset { };
            std::uint32_t m_size { };
            std::uint32_t m_elementStride { };
            std::uint32_t m_arrayCount = 1;
        };

        void doCompile() override;

        void setupLegacyBlocks() noexcept;
        void destroyLegacyBlocks() noexcept;

        [[nodiscard]] bool isLegacyMember(std::string_view uniformName) const noexcept;
        /// True when the program declares `uniformName` (or its array base) as a sampler / texel buffer.
        [[nodiscard]] bool isSamplerBinding(const std::string& uniformName) const noexcept;
        bool writeLegacy(std::string_view uniformName, const void* data, std::uint32_t size) noexcept;

        Ref<IShaderProgram> m_rhiProgram;
        ShaderReflection m_reflection;

        std::vector<LegacyBlock> m_legacyBlocks;
        std::unordered_map<std::string, std::vector<LegacyMemberRef>> m_legacyMembers;

        /// sampler uniform name -> texture unit it was told to read (useTextureBlock/useTexture)
        std::unordered_map<std::string, std::uint8_t> m_samplerUnits;

        Ref<IDescriptorSet> m_descriptorSet;
    };
}

#endif //SUNGEARENGINE_RHILEGACYSHADER_H
