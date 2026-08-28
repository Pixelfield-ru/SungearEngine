//
// Created by stuka on 24.04.2023.
//

#ifndef SUNGEARENGINE_GL46RENDERER_H
#define SUNGEARENGINE_GL46RENDERER_H

#include "SGCore/Graphics/API/GL/GL46/GL46Shader.h"
#include "SGCore/Graphics/API/GL/GLVertexArray.h"
#include "SGCore/Graphics/API/GL/GL46/GL46UniformBuffer.h"

#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"

#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"

#include <memory>
#include <unordered_map>

namespace SGCore
{
    class CoreMain;
    class GL46Device;
    class ScreenBlit;
    class ICommandList;
    class IMeshData;
    class IGPUBuffer;

    /**
     * OpenGL 4.6 backend: the GL4 implementation running on a 4.6 core context with
     * `#version 460 core` shaders. Direct-state-access paths are introduced per object type
     * during the RHI migration, once each is covered by the smoke test.
     */
    class GL46Renderer : public GL4Renderer
    {
    public:
        GL46Renderer(const GL46Renderer&) = delete;
        GL46Renderer(GL46Renderer&&) = delete;

        ~GL46Renderer() override;

        void init() noexcept override;
        bool confirmSupport() noexcept override;

        [[nodiscard]] GL46Shader* createShader() override;

        [[nodiscard]] IDevice* getDevice() noexcept override;

        /// First pass on the RHI: ScreenBlit; falls back to the legacy quad if the RHI pass
        /// failed to initialize (e.g. screen shader did not compile).
        void renderTextureOnScreen(const ITexture2D* texture, bool flipOutput,
                                   int x, int y, int width, int height) noexcept override;

        /// Meshes go through the RHI: IGPUBuffer mirror of the mesh, PSO from the bound legacy
        /// shader + cached states + the mesh vertex layout, draw via ICommandList. Falls back to
        /// the legacy VAO path when no legacy shader is bound or the mirror can not be built.
        void renderMeshData(const IMeshData* meshData, const MeshRenderState& meshRenderState) override;

        /// Legacy vertex arrays through the RHI: the array's buffers are wrapped (non-owning) as
        /// IGPUBuffers and the PSO vertex layout comes from their recorded attributes — dynamic
        /// buffers (debug lines, text, batching) keep updating in place. Falls back like renderMeshData.
        void renderArray(const Ref<IVertexArray>& vertexArray, const MeshRenderState& meshRenderState,
                         const int& verticesCount, const int& indicesCount) override;
        void renderArrayInstanced(const Ref<IVertexArray>& vertexArray, const MeshRenderState& meshRenderState,
                                  const int& verticesCount, const int& indicesCount, const int& instancesCount) override;

        /// Called by GL46Shader::bind(): the shader whose program the next RHI draw uses.
        void setCurrentLegacyShader(GL46Shader* shader) noexcept { m_currentLegacyShader = shader; }

        /// Framebuffers of this backend execute legacy bind/clear/unbind as RHI render passes.
        [[nodiscard]] GL4FrameBuffer* createFrameBuffer() override;
        /// Command list the GL46FrameBuffer objects record render-pass operations into (nullptr before init).
        [[nodiscard]] ICommandList* getFrameBufferCommandList() noexcept;

        static const std::shared_ptr<GL46Renderer>& getInstance() noexcept;

    private:
        std::unique_ptr<GL46Device> m_device;
        std::unique_ptr<ScreenBlit> m_screenBlit;
        bool m_screenBlitInitTried { };
        GL46Shader* m_currentLegacyShader { };
        Ref<ICommandList> m_meshCommandList;
        Ref<ICommandList> m_frameBufferCommandList;

        [[nodiscard]] bool prepareMeshRHI(IMeshData& meshData) noexcept;
        /// Shared body of renderArray / renderArrayInstanced; returns false when the legacy path must run.
        [[nodiscard]] bool drawLegacyArrayThroughRHI(const Ref<IVertexArray>& vertexArray, const MeshRenderState& meshRenderState,
                                                     int verticesCount, int indicesCount, int instancesCount) noexcept;
        /// Non-owning RHI wrappers of legacy GL buffers, keyed by GL name.
        std::unordered_map<std::uintptr_t, Ref<IGPUBuffer>> m_wrappedBuffers;

        GL46Renderer() noexcept = default;
    };
}

#endif //SUNGEARENGINE_GL46RENDERER_H
