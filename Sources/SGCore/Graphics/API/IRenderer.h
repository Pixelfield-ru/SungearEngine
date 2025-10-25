//
// Created by stuka on 24.04.2023.
//

#ifndef NATIVECORE_IRENDERER_H
#define NATIVECORE_IRENDERER_H

#include "GAPIType.h"
#include "IUniformBuffer.h"
#include "SGCore/Transformations/Transform.h"
#include "RenderState.h"
#include "SGCore/Memory/AssetRef.h"

namespace SGCore
{
    class LayeredFrameReceiver;
    struct Mesh;
    struct TransformBase;
    struct Atmosphere;
    struct RenderingBase;
    class IGizmo;

    class IMeshData;

    class IVertexBufferLayout;
    struct ICubemapTexture;
    class IIndexBuffer;
    class IVertexArray;
    class IVertexBuffer;
    class IFrameBuffer;
    class ITexture2D;
    class IShader;

    // todo: make description of funcs
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

        // Buffer for storing matrices of the currently main camera.
        Ref<IUniformBuffer> m_viewMatricesBuffer;
        // Buffer for storing matrices of the program.
        Ref<IUniformBuffer> m_programDataBuffer;

        /**
         * YOU MUST CALL THIS FUNCTION IN YOUR DERIVED IMPLEMENTATIONS.
         */
        virtual void init() noexcept;

        /**
         * Confirmation of GAPI support by the user's graphics card
         */
        virtual bool confirmSupport() noexcept { return false; }

        /**
         *
         * @param windowSize
         */
        virtual void prepareFrame(const glm::ivec2& windowSize) { }

        /**
         * Preparing uniform buffers for render
         * @param renderingComponent - The component that will be used as a "camera" for rendering entities.
         * @param transformComponent - The transform component of this "camera".
         */
        virtual void prepareUniformBuffers(const Ref<RenderingBase>& renderingBase,
                                           const Ref<Transform>& transform) { }

        virtual void renderMeshData(const IMeshData* meshData,
                                    const MeshRenderState& meshRenderState) { }

        virtual void renderArray(const Ref<IVertexArray>& vertexArray,
                                 const MeshRenderState& meshRenderState,
                                 const int& verticesCount,
                                 const int& indicesCount) { }
        
        virtual void renderArrayInstanced(const Ref<IVertexArray>& vertexArray,
                                          const MeshRenderState& meshRenderState,
                                          const int& verticesCount,
                                          const int& indicesCount,
                                          const int& instancesCount)
        {}

        void renderTextureOnScreen(const ITexture2D* texture, bool flipOutput = false) noexcept;

        /**
         * Applies global render state.
         * @param newBlendingState
         * @param forceState
         */
        virtual void useState(const RenderState& newRenderState, bool forceState = false) noexcept = 0;

        /**
         * Applies global blending state.
         * @param newBlendingState
         * @param forceState
         */
        virtual void useBlendingState(const BlendingState& newBlendingState, bool forceState = false) noexcept = 0;

        /**
         * Applies global mesh render state.
         * @param newMeshRenderState
         * @param forceState
         */
        virtual void useMeshRenderState(const MeshRenderState& newMeshRenderState, bool forceState = false) noexcept = 0;

        /**
         * Prints information about the graphics capabilities of the kernel on this GAPI and information about the GAPI itself.
         */
        virtual void printInfo() noexcept { }

        virtual void checkForErrors(const std::source_location& = std::source_location::current()) noexcept { }

        [[nodiscard]] virtual IShader* createShader() const = 0;

        [[nodiscard]] virtual IVertexBuffer* createVertexBuffer() const = 0;
        [[nodiscard]] virtual IVertexArray* createVertexArray() const = 0;
        [[nodiscard]] virtual IVertexBufferLayout* createVertexBufferLayout() const = 0;
        [[nodiscard]] virtual IIndexBuffer* createIndexBuffer() const = 0;
        [[nodiscard]] virtual ITexture2D* createTexture2D() const = 0;
        [[nodiscard]] virtual ICubemapTexture* createCubemapTexture() const = 0;
        [[nodiscard]] virtual IUniformBuffer* createUniformBuffer() const = 0;
        [[nodiscard]] virtual IFrameBuffer* createFrameBuffer() const = 0;

        [[nodiscard]] virtual IMeshData* createMeshData() const = 0;

        virtual void bindScreenFrameBuffer() const noexcept = 0;

        [[nodiscard]] GAPIType getGAPIType() const noexcept;

        [[nodiscard]] RenderState& getCachedRenderState() noexcept;

    protected:
        GAPIType m_apiType = SG_API_TYPE_UNKNOWN;

        RenderState m_cachedRenderState;
        MeshRenderState m_cachedMeshRenderState;
        std::array<BlendingState, 32> m_cachedColorAttachmentsBlendingStates;

        Ref<IMeshData> m_screenQuadMesh;
        AssetRef<IShader> m_screenShader;
        MeshRenderState m_screenQuadMeshRenderState;
    };
}

#endif //NATIVECORE_IRENDERER_H
