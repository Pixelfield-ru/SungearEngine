//
// Created by stuka on 24.04.2023.
//

#ifndef SUNGEARENGINE_GL4RENDERER_H
#define SUNGEARENGINE_GL4RENDERER_H

#include "SGCore/Graphics/API/GL/GL46/GL46SubPassShader.h"
#include "SGCore/Graphics/API/GL/GLVertexArray.h"
#include "SGCore/Graphics/API/GL/GLVertexBuffer.h"
#include "SGCore/Graphics/API/GL/GLVertexBufferLayout.h"
#include "SGCore/Graphics/API/GL/GLIndexBuffer.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Texture2D.h"
#include "SGCore/Graphics/API/GL/GL46/GL46UniformBuffer.h"

#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Graphics/API/GL/GL3/GL3MeshData.h"
#include "GL4FrameBuffer.h"
#include "GL4CubemapTexture.h"
#include "GL4UniformBuffer.h"

namespace SGCore
{
    class CoreMain;

    class GL4Renderer : public IRenderer
    {
    protected:
        GL4Renderer() noexcept = default;

        static inline Ref<GL4Renderer> m_instance;

    public:
        GL4Renderer(const GL4Renderer&) = delete;
        GL4Renderer(GL4Renderer&&) = delete;

        void init() noexcept override;

        bool confirmSupport() noexcept override;

        void prepareFrame(const glm::ivec2& windowSize) override;

        void prepareUniformBuffers(const RenderingBase& renderingComponent,
                                   const Ref<SGCore::Transform>& transformComponent) override;

        void renderMeshData(const Ref<IMeshData>& meshData,
                                    const MeshDataRenderInfo& meshDataRenderInfo) override;
        
        void renderArray(const Ref<IVertexArray>& vertexArray,
                         const MeshDataRenderInfo& meshDataRenderInfo,
                         const size_t& verticesCount,
                         const size_t& indicesCount) override;
        
        void renderArrayInstanced(const Ref<IVertexArray>& vertexArray,
                                  const MeshDataRenderInfo& meshDataRenderInfo,
                                  const size_t& verticesCount,
                                  const size_t& indicesCount,
                                  const size_t& instancesCount) override;

        void printInfo() noexcept override;

        /**
         * Checks for errors in GAPI.
         * @param location - Where the function is called from.
         */
        void checkForErrors(const std::source_location& location = std::source_location::current()) noexcept override;

        // TODO: create docs
        [[nodiscard]] GL46SubPassShader* createShader() const override;
        [[nodiscard]] GL46SubPassShader* createShader(const std::string& path) const override;

        [[nodiscard]] GLVertexArray* createVertexArray() const override;
        [[nodiscard]] GLVertexBuffer* createVertexBuffer() const override;
        [[nodiscard]] GLVertexBufferLayout* createVertexBufferLayout() const override;
        [[nodiscard]] GLIndexBuffer* createIndexBuffer() const override;
        [[nodiscard]] GL4Texture2D* createTexture2D() const override;
        [[nodiscard]] GL4CubemapTexture* createCubemapTexture() const override;
        [[nodiscard]] GL4UniformBuffer* createUniformBuffer() const override;
        [[nodiscard]] GL4FrameBuffer* createFrameBuffer() const override;

        [[nodiscard]] GL3MeshData* createMeshData() const override;

        // ------------- some settings for renderer ---------
        void setDepthTestingEnabled(const bool& enabled) const noexcept override;
        // --------------------------------------------------

        static const Ref<GL4Renderer>& getInstance() noexcept;
    };
}

#endif //SUNGEARENGINE_GL4RENDERER_H
