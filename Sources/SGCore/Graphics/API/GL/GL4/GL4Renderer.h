//
// Created by stuka on 24.04.2023.
//

#ifndef SUNGEARENGINE_GL4RENDERER_H
#define SUNGEARENGINE_GL4RENDERER_H

#include "SGCore/Logging/Log.h"

#include "SGCore/Graphics/API/GL/GL46/GL46Shader.h"
#include "SGCore/Graphics/API/GL/GLVertexArray.h"
#include "SGCore/Graphics/API/GL/GLVertexBuffer.h"
#include "SGCore/Graphics/API/GL/GLVertexBufferLayout.h"
#include "SGCore/Graphics/API/GL/GLIndexBuffer.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Texture2D.h"
#include "SGCore/Graphics/API/GL/GL46/GL46UniformBuffer.h"

#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/ImportedScenesArch/IMesh.h"
#include "SGCore/Graphics/API/GL/GL3/GL3Mesh.h"
#include "GL4FrameBuffer.h"
#include "GL4CubemapTexture.h"
#include "GL4UniformBuffer.h"

namespace Core::Main
{
    class CoreMain;
}

namespace Core::Graphics
{
    class GL4Renderer : public IRenderer
    {
    protected:
        // Buffer for storing matrices of the currently rendered model.
        std::shared_ptr<GL4UniformBuffer> m_modelMatricesBuffer;
        // stores material colors, coeffs, but not samplers
        std::shared_ptr<GL4UniformBuffer> m_materialDataBuffer;
        // Buffer for storing matrices of the currently main camera.
        std::shared_ptr<GL4UniformBuffer> m_viewMatricesBuffer;
        // Buffer for storing matrices of the program.
        std::shared_ptr<GL4UniformBuffer> m_programDataBuffer;

        GL4Renderer() noexcept = default;

        static inline std::shared_ptr<GL4Renderer> m_instance;

    public:
        GL4Renderer(const GL4Renderer&) = delete;
        GL4Renderer(GL4Renderer&&) = delete;

        void init() noexcept override;

        bool confirmSupport() noexcept override;

        void prepareFrame(const glm::ivec2& windowSize) override;

        void prepareUniformBuffers(const std::shared_ptr<ECS::IRenderingComponent>& renderingComponent,
                                   const std::shared_ptr<ECS::TransformComponent>& transformComponent) override;

        void renderMesh(const std::shared_ptr<ECS::TransformComponent>& transformComponent,
                        const std::shared_ptr<ECS::MeshComponent>& meshComponent) override;

        void renderPrimitive(const std::shared_ptr<ECS::TransformComponent>& transformComponent,
                             const std::shared_ptr<ECS::IPrimitiveComponent>& primitiveComponent) override;

        void renderFrameBufferOnMesh(const std::shared_ptr<IFrameBuffer>& frameBuffer,
                                     const std::shared_ptr<ImportedScene::IMesh>& mesh) override;

        void printInfo() noexcept override;

        /**
         * Checks for errors in GAPI.
         * @param location - Where the function is called from.
         */
        void checkForErrors(const std::source_location& location = std::source_location::current()) noexcept override;

        // TODO: create docs
        [[nodiscard]] GL46Shader* createShader() override;
        [[nodiscard]] GL46Shader* createShader(const std::string& path) override;

        [[nodiscard]] GLVertexArray* createVertexArray() override;
        [[nodiscard]] GLVertexBuffer* createVertexBuffer() override;
        [[nodiscard]] GLVertexBufferLayout* createVertexBufferLayout() override;
        [[nodiscard]] GLIndexBuffer* createIndexBuffer() override;
        [[nodiscard]] GL4Texture2D* createTexture2D() override;
        [[nodiscard]] GL4CubemapTexture* createCubemapTexture() override;
        [[nodiscard]] GL4UniformBuffer* createUniformBuffer() override;
        [[nodiscard]] GL4FrameBuffer* createFrameBuffer() override;

        [[nodiscard]] GL3Mesh* createMesh() override;

        static const std::shared_ptr<GL4Renderer>& getInstance() noexcept;
    };
}

#endif //SUNGEARENGINE_GL4RENDERER_H
