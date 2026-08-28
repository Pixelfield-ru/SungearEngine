//
// Created by 8bitniksis on 17.08.2026.
//
// Vertical slice of the RHI on the GL46 backend: a triangle drawn through IDevice /
// ICommandList into a legacy IFrameBuffer, its color fed through the SGSLEVulkanizer legacy
// uniform block by reflection, read back and checked. No reference images — exact pixel
// values are asserted.

#include <cstdio>
#include <cmath>
#include <utility>
#include <cstring>
#include <csignal>
#include <stacktrace>
#include <vector>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "SGCore/Graphics/API/GAPISelector.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/RHI/IDevice.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Main/Window.h"
#include "SGCore/Utils/SGSL/SGSLEVulkanizer.h"

namespace
{
    int g_failures = 0;
    int g_exitCode = 0;
    bool g_done = false;

    void check(bool condition, const char* what) noexcept
    {
        if(!condition)
        {
            ++g_failures;
            std::printf("  FAIL: %s\n", what);
        }
    }

    // vertex: position (2 floats) + color (3 floats)
    struct Vertex
    {
        float m_x, m_y;
        float m_r, m_g, m_b;
    };

    // legacy-style GLSL: a loose uniform, no bindings — exactly what engine shaders look like
    // struct-typed loose uniforms are how the engine passes per-object data
    // ("objectTransform.modelMatrix"), so the slice carries one: both reflections must report its
    // leaves by dotted path or every write to them silently misses
    const char* vertex_source =
        "layout(location = 0) in vec2 positionAttribute;\n"
        "layout(location = 1) in vec3 colorAttribute;\n"
        "struct SGTestTransform { mat4 modelMatrix; vec4 tint; };\n"
        "uniform SGTestTransform testTransform;\n"
        "uniform vec2 u_offset;\n"
        "out vec3 vs_color;\n"
        "void main() { vs_color = colorAttribute * testTransform.tint.rgb;\n"
        "  gl_Position = testTransform.modelMatrix * vec4(positionAttribute + u_offset, 0.0, 1.0); }\n";

    const char* fragment_source =
        "in vec3 vs_color;\n"
        "uniform vec4 u_tint;\n"
        "void main() { gl_FragColor = vec4(vs_color, 1.0) * u_tint; }\n";

    void runLegacyDrawSlice(const SGCore::Ref<SGCore::IFrameBuffer>& sourceFrameBuffer);

    void runSlice()
    {
        auto* device = SGCore::CoreMain::getRenderer()->getDevice();
        check(device != nullptr, "renderer exposes an RHI device");
        if(!device) { g_exitCode = 2; return; }

        const auto& properties = device->getProperties();
        std::printf("device: api=%s originBottomLeft=%d framesInFlight=%u\n",
                    std::string(gapiTypeToString(properties.m_apiType)).c_str(),
                    properties.m_originBottomLeft, properties.m_framesInFlight);

        // ---- shader: vulkanize (OpenGL dialect) then create program
        std::vector<SGCore::SGSLEVulkanizer::Stage> stages = {
            { SGCore::SST_VERTEX, vertex_source },
            { SGCore::SST_FRAGMENT, fragment_source },
        };
        SGCore::SGSLEVulkanizer::Config vulkanizeConfig;
        // OPENGL dialect for the GL backends, VULKAN (explicit set/binding, SPIR-V compiled by the
        // device) for the explicit ones
        vulkanizeConfig.m_target = isOpenGLAPI(properties.m_apiType) ? SGCore::SGSLEVulkanizer::Target::OPENGL
                                                                     : SGCore::SGSLEVulkanizer::Target::VULKAN;
        const auto report = SGCore::SGSLEVulkanizer::vulkanize(stages, vulkanizeConfig);
        // u_offset and testTransform in the vertex stage, u_tint in the fragment one
        check(report.m_looseUniformsMoved == 3, "3 loose uniforms moved into per-stage blocks");

        SGCore::ShaderProgramDesc programDesc;
        programDesc.m_debugName = "rhi_triangle";
        for(const auto& stage : stages) programDesc.m_stages.push_back({ stage.m_type, stage.m_code, { } });
        auto program = device->createShaderProgram(programDesc);
        check(program && program->isValid(), "shader program compiles and links");
        if(!program || !program->isValid())
        {
            std::printf("%s\n", program ? program->getLog().c_str() : "(no program)");
            g_exitCode = 2;
            return;
        }

        const auto& reflection = program->getReflection();
        const auto* vsBlock = reflection.findBinding("SGLegacyUniforms_vertex");
        const auto* fsBlock = reflection.findBinding("SGLegacyUniforms_fragment");
        check(vsBlock && fsBlock, "reflection reports both legacy blocks");
        const auto* offsetMember = reflection.findMember("SGLegacyUniforms_vertex", "u_offset");
        const auto* tintMember = reflection.findMember("SGLegacyUniforms_fragment", "u_tint");
        check(offsetMember && tintMember, "reflection reports block members");
        check(reflection.m_vertexInputs.size() == 2, "reflection reports 2 vertex inputs");
        if(!vsBlock || !fsBlock || !offsetMember || !tintMember) { g_exitCode = 2; return; }
        std::printf("reflection: vs block binding=%u size=%u, fs block binding=%u size=%u, u_offset@%u, u_tint@%u\n",
                    vsBlock->m_binding, vsBlock->m_blockSize, fsBlock->m_binding, fsBlock->m_blockSize,
                    offsetMember->m_offset, tintMember->m_offset);
        check(vsBlock->m_binding != fsBlock->m_binding, "per-stage blocks have distinct bindings");

        // GL reports leaves natively (GL_UNIFORM resources); SPIR-V reports the struct itself and
        // needs flattening, so this is where the two reflections used to disagree
        const auto* nestedMatrix = reflection.findMember("SGLegacyUniforms_vertex", "testTransform.modelMatrix");
        const auto* nestedTint = reflection.findMember("SGLegacyUniforms_vertex", "testTransform.tint");
        check(nestedMatrix && nestedTint, "reflection reports nested struct members by dotted path");
        if(nestedMatrix && nestedTint)
        {
            std::printf("nested members: testTransform.modelMatrix@%u size=%u, testTransform.tint@%u size=%u\n",
                        nestedMatrix->m_offset, nestedMatrix->m_size, nestedTint->m_offset, nestedTint->m_size);
            check(nestedTint->m_offset >= nestedMatrix->m_offset + 64,
                  "nested member offsets are relative to the block, not to their struct");
        }

        // ---- uniform data by reflection
        SGCore::GPUBufferDesc uboDesc;
        uboDesc.m_usage = SGCore::GPUBufferUsage::SGG_UNIFORM_BUFFER;
        uboDesc.m_access = SGCore::GPUMemoryAccess::SGG_HOST_VISIBLE;

        uboDesc.m_size = vsBlock->m_blockSize;
        uboDesc.m_debugName = "vs_legacy";
        auto vsUbo = device->createBuffer(uboDesc);
        const float offset[2] = { 0.0f, 0.0f };
        vsUbo->write(offset, sizeof(offset), offsetMember->m_offset);
        if(nestedMatrix && nestedTint)
        {
            // the triangle degenerates unless the nested matrix really lands where reflection says
            const float identity[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
            const float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            vsUbo->write(identity, sizeof(identity), nestedMatrix->m_offset);
            vsUbo->write(white, sizeof(white), nestedTint->m_offset);
        }

        uboDesc.m_size = fsBlock->m_blockSize;
        uboDesc.m_debugName = "fs_legacy";
        auto fsUbo = device->createBuffer(uboDesc);
        const float tint[4] = { 1.0f, 0.5f, 1.0f, 1.0f };
        fsUbo->write(tint, sizeof(tint), tintMember->m_offset);

        auto descriptorSet = device->createDescriptorSet();
        descriptorSet->setUniformBuffer(vsBlock->m_binding, vsUbo);
        descriptorSet->setUniformBuffer(fsBlock->m_binding, fsUbo);

        // ---- geometry: one triangle covering the center, colored green
        const Vertex vertices[3] = {
            { -0.8f, -0.8f, 0.0f, 1.0f, 0.0f },
            {  0.8f, -0.8f, 0.0f, 1.0f, 0.0f },
            {  0.0f,  0.8f, 0.0f, 1.0f, 0.0f },
        };
        SGCore::GPUBufferDesc vboDesc;
        vboDesc.m_size = sizeof(vertices);
        vboDesc.m_usage = SGCore::GPUBufferUsage::SGG_VERTEX_BUFFER;
        vboDesc.m_access = SGCore::GPUMemoryAccess::SGG_DEVICE_LOCAL;
        vboDesc.m_debugName = "triangle_vbo";
        auto vbo = device->createBuffer(vboDesc);

        // ---- pipeline
        SGCore::PipelineStateDesc pipelineDesc;
        pipelineDesc.m_program = program;
        pipelineDesc.m_debugName = "rhi_triangle";
        pipelineDesc.m_renderState.m_useDepthTest = false;
        pipelineDesc.m_renderState.m_useStencilTest = false;
        pipelineDesc.m_blendingState.m_useBlending = false;
        pipelineDesc.m_meshRenderState.m_useFacesCulling = false;
        pipelineDesc.m_meshRenderState.m_useIndices = false;
        pipelineDesc.m_vertexInput.m_slots = { { 0, sizeof(Vertex), false } };
        pipelineDesc.m_vertexInput.m_attributes = {
            { 0, 0, SGGDataType::SGG_FLOAT, 2, offsetof(Vertex, m_x), false },
            { 1, 0, SGGDataType::SGG_FLOAT, 3, offsetof(Vertex, m_r), false },
        };
        auto pipeline = device->getOrCreatePipeline(pipelineDesc);
        check(pipeline != nullptr, "pipeline created");
        check(device->getOrCreatePipeline(pipelineDesc) == pipeline, "pipeline cache returns the same object for an equal desc");

        // ---- render target: legacy framebuffer 64x64 RGBA8
        constexpr int size = 64;
        SGCore::Ref<SGCore::IFrameBuffer> frameBuffer(SGCore::CoreMain::getRenderer()->createFrameBuffer());
        frameBuffer->setSize(size, size);
        frameBuffer->create();
        frameBuffer->bind();
        frameBuffer->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0,
                                   SGGColorFormat::SGG_RGBA, SGGColorInternalFormat::SGG_RGBA8, SGGDataType::SGG_UNSIGNED_BYTE, 0, 0);
        frameBuffer->unbind();

        // ---- record + execute
        auto commandList = device->createCommandList();
        commandList->begin();

        SGCore::RenderPassBeginDesc pass;
        pass.m_frameBuffer = frameBuffer.get();
        pass.m_colorAttachments = { SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 };
        pass.m_colorLoadOp = SGCore::LoadOp::SGG_CLEAR;
        pass.m_clearColor = { 0.0f, 0.0f, 1.0f, 1.0f };
        pass.m_width = size;
        pass.m_height = size;
        commandList->beginRenderPass(pass);

        commandList->uploadData(vbo, vertices, sizeof(vertices));
        commandList->bindPipeline(pipeline);
        commandList->bindDescriptorSet(0, descriptorSet);
        commandList->bindVertexBuffer(0, vbo);
        commandList->setViewport({ 0.0f, 0.0f, float(size), float(size) });
        commandList->draw(3);

        commandList->endRenderPass();
        commandList->end();
        device->submit(commandList);
        device->waitIdle();

        // ---- verify: center pixel = green * tint (0, 0.5, 0), corner = clear color (blue)
        SGCore::AttachmentReadback readback;
        check(frameBuffer->readAttachmentPixels(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0, readback), "readback works");
        if(readback.m_data.empty()) { g_exitCode = 2; return; }
        check(readback.m_format == SGGColorFormat::SGG_RGBA && readback.m_dataType == SGGDataType::SGG_UNSIGNED_BYTE, "readback in RGBA8 as the attachment was declared");

        auto pixel = [&](int x, int y) {
            const std::size_t index = (static_cast<std::size_t>(y) * size + x) * 4;
            return glm::ivec4(readback.m_data[index], readback.m_data[index + 1], readback.m_data[index + 2], readback.m_data[index + 3]);
        };

        const auto center = pixel(size / 2, size / 2 - 4);
        const auto corner = pixel(1, size - 2);
        std::printf("center=(%d,%d,%d,%d) corner=(%d,%d,%d,%d)\n", center.r, center.g, center.b, center.a, corner.r, corner.g, corner.b, corner.a);
        check(center.r == 0 && center.g >= 126 && center.g <= 129 && center.b == 0 && center.a == 255, "center pixel is green * tint (0, ~128, 0)");
        check(corner.r == 0 && corner.g == 0 && corner.b == 255 && corner.a == 255, "corner pixel is the clear color");

        // ---- the engine's own drawing path: legacy shader + renderMeshData through the facade
        runLegacyDrawSlice(frameBuffer);

        // ---- legacy framebuffer facade: bind -> clear -> unbind on the renderer's shared command
        // list, then read back. This is the path every engine render pass takes, and it is the one
        // that leaves the smoke scene empty on Vulkan, so it gets its own fast reproduction here.
        {
            SGCore::Ref<SGCore::IFrameBuffer> facadeBuffer(SGCore::CoreMain::getRenderer()->createFrameBuffer());
            facadeBuffer->setSize(size, size);
            facadeBuffer->create();
            facadeBuffer->bind();
            facadeBuffer->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0,
                                        SGGColorFormat::SGG_RGBA, SGGColorInternalFormat::SGG_RGBA8, SGGDataType::SGG_UNSIGNED_BYTE, 0, 0);
            facadeBuffer->unbind();

            // the clear colour a pass would use
            if(const auto attachment = facadeBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0))
            {
                attachment->m_clearColor = { 1.0f, 0.0f, 0.0f, 1.0f };
            }

            facadeBuffer->bind();
            facadeBuffer->bindAttachmentsToDrawIn(std::vector<SGFrameBufferAttachmentType> { SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 });
            facadeBuffer->clearAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0);
            facadeBuffer->unbind();

            SGCore::AttachmentReadback facadeReadback;
            const bool facadeRead = facadeBuffer->readAttachmentPixels(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0, facadeReadback);
            check(facadeRead && !facadeReadback.m_data.empty(), "legacy framebuffer facade readback works");
            if(facadeRead && facadeReadback.m_data.size() >= 4)
            {
                const glm::ivec4 facadePixel(facadeReadback.m_data[0], facadeReadback.m_data[1], facadeReadback.m_data[2], facadeReadback.m_data[3]);
                std::printf("facade clear pixel=(%d,%d,%d,%d)\n", facadePixel.r, facadePixel.g, facadePixel.b, facadePixel.a);
                check(facadePixel.r == 255 && facadePixel.g == 0 && facadePixel.b == 0 && facadePixel.a == 255,
                      "clear issued through the framebuffer facade reaches the attachment");
            }
        }

        // ---- screen blit (first migrated pass): the attachment onto the window at 1:1, read the window back
        auto* renderer = SGCore::CoreMain::getRenderer().get();
        renderer->bindScreenFrameBuffer();
        renderer->renderTextureOnScreen(frameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0).get(), false, 0, 0, size, size);
        device->waitIdle();

        SGCore::AttachmentReadback screen;
        const bool screenRead = renderer->readScreenPixels(screen);
        check(screenRead, "screen readback works");
        if(screenRead && screen.m_width >= size && screen.m_height >= size)
        {
            auto screenPixel = [&](int x, int y) {
                const std::size_t index = (static_cast<std::size_t>(y) * screen.m_width + x) * 4;
                return glm::ivec4(screen.m_data[index], screen.m_data[index + 1], screen.m_data[index + 2], screen.m_data[index + 3]);
            };
            const auto screenCenter = screenPixel(size / 2, size / 2 - 4);
            const auto screenCorner = screenPixel(1, size - 2);
            std::printf("screen center=(%d,%d,%d,%d) corner=(%d,%d,%d,%d)\n",
                        screenCenter.r, screenCenter.g, screenCenter.b, screenCenter.a,
                        screenCorner.r, screenCorner.g, screenCorner.b, screenCorner.a);
            check(screenCenter == center, "screen blit reproduces the attachment center pixel");
            check(screenCorner == corner, "screen blit reproduces the attachment corner pixel");
        }
    }

    /// Reproduces the shape of a real engine frame on the legacy facades: several framebuffers with
    /// several attachments, cleared and re-bound in the order LayeredFrameReceiver + IRenderPass use
    /// them, with binds that overlap (a framebuffer is bound while another is still bound). The
    /// smoke scene renders nothing on Vulkan while the single-framebuffer slice above passes, so the
    /// difference has to live in this sequencing.
    void runFacadeSequenceSlice()
    {
        auto* renderer = SGCore::CoreMain::getRenderer().get();
        constexpr int size = 64;

        auto makeBuffer = [&](int attachmentsCount) {
            SGCore::Ref<SGCore::IFrameBuffer> buffer(renderer->createFrameBuffer());
            buffer->setSize(size, size);
            buffer->create();
            buffer->bind();
            for(int i = 0; i < attachmentsCount; ++i)
            {
                const auto type = static_cast<SGFrameBufferAttachmentType>(
                    std::to_underlying(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0) + i);
                buffer->addAttachment(type, SGGColorFormat::SGG_RGBA, SGGColorInternalFormat::SGG_RGBA8,
                                      SGGDataType::SGG_UNSIGNED_BYTE, 0, 0);
            }
            buffer->unbind();
            return buffer;
        };

        // the layers framebuffer of a camera carries 8 attachments, the FX one fewer
        auto layers = makeBuffer(8);
        auto fx = makeBuffer(4);

        auto colorOf = [](int attachmentIndex) {
            return glm::vec4(attachmentIndex % 2 ? 0.0f : 1.0f, attachmentIndex >= 4 ? 1.0f : 0.0f, 0.25f, 1.0f);
        };
        for(int i = 0; i < 8; ++i)
        {
            const auto type = static_cast<SGFrameBufferAttachmentType>(
                std::to_underlying(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0) + i);
            if(const auto attachment = layers->getAttachment(type)) attachment->m_clearColor = colorOf(i);
        }

        std::vector<SGFrameBufferAttachmentType> allLayerAttachments;
        for(int i = 0; i < 8; ++i)
        {
            allLayerAttachments.push_back(static_cast<SGFrameBufferAttachmentType>(
                std::to_underlying(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0) + i));
        }

        // 1. LayeredFrameReceiver::clearPostProcessFrameBuffers(). The draw set has to be declared
        //    before clearing: a clear addresses the pass's draw buffers by index (glClearBufferfv on
        //    GL, vkCmdClearAttachments on Vulkan), not the attachment slots.
        layers->bind();
        layers->bindAttachmentsToDrawIn(allLayerAttachments);
        layers->clear();
        layers->unbind();

        fx->bind();
        fx->clear();
        fx->unbind();

        // 2. IRenderPass::iterateCameras(): bind, then narrow the draw set — and, while the layers
        //    framebuffer is still bound, another pass binds its own target (shadow maps do this)
        layers->bind();
        layers->bindAttachmentsToDrawIn(allLayerAttachments);

        fx->bind();      // overlapping bind: nobody unbound the layers framebuffer
        fx->unbind();

        layers->unbind();

        // 3. the capture: what survived in the layers framebuffer?
        for(const int index : { 1, 5 })
        {
            const auto type = static_cast<SGFrameBufferAttachmentType>(
                std::to_underlying(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0) + index);

            SGCore::AttachmentReadback readback;
            if(!layers->readAttachmentPixels(type, readback) || readback.m_data.size() < 4)
            {
                check(false, "sequenced facade readback works");
                continue;
            }

            const glm::ivec4 pixel(readback.m_data[0], readback.m_data[1], readback.m_data[2], readback.m_data[3]);
            const auto expected = colorOf(index);
            const glm::ivec4 want(int(expected.r * 255.0f + 0.5f), int(expected.g * 255.0f + 0.5f),
                                  int(expected.b * 255.0f + 0.5f), int(expected.a * 255.0f + 0.5f));
            std::printf("sequenced attachment %d: got=(%d,%d,%d,%d) want=(%d,%d,%d,%d)\n", index,
                        pixel.r, pixel.g, pixel.b, pixel.a, want.r, want.g, want.b, want.a);

            const bool matches = std::abs(pixel.r - want.r) <= 1 && std::abs(pixel.g - want.g) <= 1 &&
                                 std::abs(pixel.b - want.b) <= 1 && std::abs(pixel.a - want.a) <= 1;
            check(matches, "clears survive a frame-shaped sequence of framebuffer binds");
        }
    }

    /// The last untested link of the engine's real drawing path: a legacy IShader (an actual engine
    /// .sgshader, compiled through the backend's facade) drawing a legacy IMeshData with
    /// IRenderer::renderMeshData into a framebuffer opened by the framebuffer facade. Everything
    /// below this (device, command list, framebuffer facade, reflection) is already covered; the
    /// smoke scene renders nothing on Vulkan, so the failure has to be here.
    void runLegacyDrawSlice(const SGCore::Ref<SGCore::IFrameBuffer>& sourceFrameBuffer)
    {
        auto* renderer = SGCore::CoreMain::getRenderer().get();
        constexpr int size = 64;

        const auto screenShader = renderer->m_screenShader;
        check(static_cast<bool>(screenShader), "engine screen shader is loaded");
        if(!screenShader) return;

        // full-screen quad, built the same way IRenderer::init() builds its own
        SGCore::Ref<SGCore::IMeshData> quad(renderer->createMeshData());
        quad->m_vertices.resize(4);
        quad->m_vertices[0] = { .m_position = { -1, -1, 0 }, .m_uv = { 0, 0, 0 }, .m_normal = { 0, 1, 0 } };
        quad->m_vertices[1] = { .m_position = { -1,  1, 0 }, .m_uv = { 0, 1, 0 }, .m_normal = { 0, 1, 0 } };
        quad->m_vertices[2] = { .m_position = {  1,  1, 0 }, .m_uv = { 1, 1, 0 }, .m_normal = { 0, 1, 0 } };
        quad->m_vertices[3] = { .m_position = {  1, -1, 0 }, .m_uv = { 1, 0, 0 }, .m_normal = { 0, 1, 0 } };
        quad->m_indices = { 0, 2, 1, 0, 3, 2 };
        quad->prepare();

        // shaped like a camera's layers framebuffer: several colour attachments plus depth, which is
        // what the geometry pass renders into
        SGCore::Ref<SGCore::IFrameBuffer> target(renderer->createFrameBuffer());
        target->setSize(size, size);
        target->create();
        target->bind();
        std::vector<SGFrameBufferAttachmentType> drawIn;
        for(int i = 0; i < 8; ++i)
        {
            const auto type = static_cast<SGFrameBufferAttachmentType>(
                std::to_underlying(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0) + i);
            target->addAttachment(type, SGGColorFormat::SGG_RGBA, SGGColorInternalFormat::SGG_RGBA8,
                                  SGGDataType::SGG_UNSIGNED_BYTE, 0, 0);
            drawIn.push_back(type);
        }
        target->addAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                              SGGColorFormat::SGG_DEPTH_COMPONENT, SGGColorInternalFormat::SGG_DEPTH_COMPONENT32,
                              SGGDataType::SGG_FLOAT, 0, 0);
        target->unbind();

        SGCore::MeshRenderState quadState;
        quadState.m_useFacesCulling = false;
        quadState.m_useIndices = true;

        target->bind();
        target->bindAttachmentsToDrawIn(drawIn);
        target->clear();

        screenShader->bind();
        screenShader->useInteger("u_flipOutput", 0);
        // the source of the blit: the attachment the triangle slice rendered
        screenShader->useTextureBlock("u_bufferToDisplay", 0);
        if(const auto source = sourceFrameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0))
        {
            source->bind(0);
        }

        renderer->renderMeshData(quad.get(), quadState);
        target->unbind();

        SGCore::AttachmentReadback readback;
        if(!target->readAttachmentPixels(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0, readback) || readback.m_data.size() < 4)
        {
            check(false, "legacy draw readback works");
            return;
        }

        auto pixel = [&](int x, int y) {
            const std::size_t index = (static_cast<std::size_t>(y) * size + x) * 4;
            return glm::ivec4(readback.m_data[index], readback.m_data[index + 1], readback.m_data[index + 2], readback.m_data[index + 3]);
        };
        const auto center = pixel(size / 2, size / 2 - 4);
        const auto corner = pixel(1, size - 2);
        std::printf("legacy draw: center=(%d,%d,%d,%d) corner=(%d,%d,%d,%d)\n",
                    center.r, center.g, center.b, center.a, corner.r, corner.g, corner.b, corner.a);

        // the quad samples the source attachment 1:1, so the result must reproduce it
        check(center.g >= 126 && center.g <= 129 && center.r == 0 && center.b == 0,
              "legacy IShader + renderMeshData reproduces the sampled center pixel");
        check(corner.b == 255 && corner.r == 0 && corner.g == 0,
              "legacy IShader + renderMeshData reproduces the sampled corner pixel");
    }

    void onInit()
    {
        runSlice();
        runFacadeSequenceSlice();
        std::printf("\nRHI slice: %s (%d failure(s))\n", g_failures == 0 && g_exitCode == 0 ? "PASS" : "FAIL", g_failures);
        if(g_failures != 0 && g_exitCode == 0) g_exitCode = 1;
        g_done = true;
        SGCore::CoreMain::getWindow().setShouldClose(true);
    }

    void onUpdate(double, double)
    {
        if(g_done) SGCore::CoreMain::getWindow().setShouldClose(true);
    }
}

namespace
{
    // an abort() at exit (VMA/driver asserts in Debug) is otherwise invisible in CI logs
    void onAbort(int)
    {
        std::fprintf(stderr, "abort() called, stack:\n%s\n", std::to_string(std::stacktrace::current()).c_str());
        std::fflush(stderr);
    }
}

int main(int argc, char** argv)
{
    std::signal(SIGABRT, onAbort);
    // unbuffered so crash-handler output survives when stdout is a file/pipe
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    std::setvbuf(stderr, nullptr, _IONBF, 0);
    for(int i = 1; i < argc; ++i)
    {
        if(std::strcmp(argv[i], "--gapi") == 0 && i + 1 < argc)
        {
            if(const auto type = SGCore::gapiTypeFromString(argv[++i])) SGCore::GAPISelector::setPreference({ *type });
        }
    }

    SGCore::CoreMain::onInit.connect<&onInit>();
    SGCore::CoreMain::getRenderTimer().onUpdate.connect<&onUpdate>();

    SGCore::CoreMain::init();

    SGCore::CoreMain::startCycle();

    return g_exitCode;
}
