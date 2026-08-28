//
// Created by 8bitniksis on 18.08.2026.
//

#pragma once

#if defined(_WIN32)

#include <memory>

#include "SGCore/Graphics/API/IRenderer.h"
#include "RHI/DX12Context.h"
#include "RHI/DX12Device.h"
#include "SGCore/Graphics/RHI/RHIIndexBuffer.h"
#include "SGCore/Graphics/RHI/RHIUniformBuffer.h"
#include "SGCore/Graphics/RHI/RHIVertexArray.h"
#include "SGCore/Graphics/RHI/RHIVertexBuffer.h"
#include "DX12CubemapTexture.h"
#include "DX12FrameBuffer.h"
#include "DX12Shader.h"
#include "DX12Texture2D.h"

namespace SGCore
{
    class ScreenBlit;

    /**
     * The DX12 renderer (stage 3 of docs/IMPLEMENTATION_PLAN.md).
     *
     * Brings up the device (adapter, D3D12 device, direct queue, debug layer), the swapchain and the
     * RHI on top of them: DX12Device / DX12CommandList / DX12PipelineState, mapped one to one onto
     * the Vulkan backend. What is still missing is the shader path (SPIR-V -> HLSL -> DXIL, task 3.4)
     * and the legacy IShader / ITexture2D / IFrameBuffer facades built on it (task 3.5), so the
     * IRenderer factories return nullptr and a full engine run stops right after init(); RHI-level
     * use (device, swapchain, command lists, buffers) already works. The backend stays out of
     * GAPISelector's default order, exactly as the Vulkan backend did while it was being built.
     */
    class DX12Renderer : public IRenderer
    {
    private:
        DX12Renderer() noexcept = default;

        static inline std::shared_ptr<DX12Renderer> m_instance;

    public:
        DX12Renderer(const DX12Renderer&) = delete;
        DX12Renderer(DX12Renderer&&) = delete;
        ~DX12Renderer() override;

        void init() noexcept override;
        bool confirmSupport() noexcept final;

        void prepareFrame(const glm::ivec2& windowSize) override;
        void printInfo() noexcept override;
        void checkForErrors(const std::source_location& location = std::source_location::current()) noexcept override;

        void renderMeshData(const IMeshData* meshData, const MeshRenderState& meshRenderState) override;
        void renderArray(const Ref<IVertexArray>& vertexArray, const MeshRenderState& meshRenderState,
                         const int& verticesCount, const int& indicesCount) override;
        void renderArrayInstanced(const Ref<IVertexArray>& vertexArray, const MeshRenderState& meshRenderState,
                                  const int& verticesCount, const int& indicesCount, const int& instancesCount) override;

        void useState(const RenderState& newRenderState, bool forceState = false) noexcept final;
        void useBlendingState(const BlendingState& newBlendingState, bool forceState = false) noexcept final;
        void useMeshRenderState(const MeshRenderState& newMeshRenderState, bool forceState = false) noexcept final;

        // ---- resource factories: nullptr until task 3.3 backs them with the RHI
        [[nodiscard]] IShader* createShader() override;
        [[nodiscard]] IVertexArray* createVertexArray() override;
        [[nodiscard]] IVertexBuffer* createVertexBuffer() override;
        [[nodiscard]] IIndexBuffer* createIndexBuffer() override;
        [[nodiscard]] ITexture2D* createTexture2D() override;
        [[nodiscard]] ICubemapTexture* createCubemapTexture() override;
        [[nodiscard]] IUniformBuffer* createUniformBuffer() override;
        [[nodiscard]] IFrameBuffer* createFrameBuffer() override;
        [[nodiscard]] IMeshData* createMeshData() const override;

        void bindScreenFrameBuffer() const noexcept final;
        void setViewport(int x, int y, int width, int height) const noexcept final;

        IGPUObjectsStorage& storage() noexcept final;
        const IGPUObjectsStorage& storage() const noexcept final;

        void reload() noexcept override;
        void shutdown() noexcept override;

        void renderTextureOnScreen(const ITexture2D* texture, bool flipOutput, int x, int y, int width, int height) noexcept override;
        [[nodiscard]] bool readScreenPixels(AttachmentReadback& out) const noexcept override;

        /// The command list legacy DX12FrameBuffer::bind()/unbind() record into (one per renderer).
        [[nodiscard]] ICommandList* getFrameBufferCommandList() noexcept;
        [[nodiscard]] const Ref<ICommandList>& getFrameBufferCommandListRef() noexcept;

        [[nodiscard]] IDevice* getDevice() noexcept override;
        [[nodiscard]] DX12Device* getDX12Device() const noexcept { return m_device.get(); }
        [[nodiscard]] DX12Context& getContext() noexcept { return *m_context; }

        /// The live device, or nullptr once shutdown() ran (or before init()). Legacy facades must
        /// use this instead of the singleton: assets outlive the renderer and are destroyed in static
        /// destruction, when the singleton itself may already be gone — the lesson the Vulkan backend
        /// paid for with an abort() at exit.
        [[nodiscard]] static DX12Device* getLiveDevice() noexcept;

        /// The shader the passes bound last: DX12 has no "current program", so draws take the program
        /// (and its legacy uniform blocks / sampler table) from here, as GL46 and Vulkan do.
        void setCurrentLegacyShader(DX12Shader* shader) noexcept { m_currentLegacyShader = shader; }
        [[nodiscard]] DX12Shader* getCurrentLegacyShader() const noexcept { return m_currentLegacyShader; }

        static const std::shared_ptr<DX12Renderer>& getInstance() noexcept;

    private:
        std::shared_ptr<DX12Context> m_context = std::make_shared<DX12Context>();
        std::unique_ptr<DX12Device> m_device;
        Ref<ICommandList> m_frameBufferCommandList;
        DX12Shader* m_currentLegacyShader { };
        std::unique_ptr<ScreenBlit> m_screenBlit;
        bool m_screenBlitInitTried { };

        RenderState m_cachedRenderState { };
        MeshRenderState m_cachedMeshRenderState { };

        static inline DX12Device* s_liveDevice = nullptr;

    };
}

#endif
