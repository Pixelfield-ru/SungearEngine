//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GL46RENDERER_H
#define SUNGEARENGINE_GL46RENDERER_H

#include "OpenGL/include/glad/glad.h"

#include "SGCore/Logging/Log.h"
#include "SGCore/Main/Core.h"

#include "GL46Shader.h"
#include "SGCore/Graphics/API/GL/GLVertexArray.h"
#include "SGCore/Graphics/API/GL/GLVertexBuffer.h"
#include "SGCore/Graphics/API/GL/GLVertexBufferLayout.h"
#include "SGCore/Graphics/API/GL/GLIndexBuffer.h"
#include "GL46Texture2D.h"
#include "GL46UniformBuffer.h"

#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/ImportedScenesArch/IMesh.h"

namespace Core::Graphics::GL
{
    class GL46Renderer : public IRenderer
    {
    private:
        // Buffer for storing matrices of the currently rendered model.
        std::shared_ptr<IUniformBuffer> m_modelMatricesBuffer;
        // Buffer for storing matrices of the currently main camera.
        std::shared_ptr<IUniformBuffer> m_cameraMatricesBuffer;

        GL46Renderer() noexcept = default;

        //GL46Renderer() noexcept = default;

        static inline std::shared_ptr<GL46Renderer> m_instance;

    public:
        GL46Renderer(const GL46Renderer&) = delete;
        GL46Renderer(GL46Renderer&&) = delete;

        void init() noexcept override;

        void renderFrame(const glm::ivec2& windowSize) override;

        void renderMesh(const std::shared_ptr<ECS::CameraComponent>& cameraComponent,
                        const std::shared_ptr<ECS::TransformComponent>& transformComponent,
                        const std::shared_ptr<ECS::MeshComponent>& meshComponent) override;

        void printInfo() noexcept override;

        /**
         * Checks for errors in GAPI.
         * @param location - Where the function is called from.
         */
        void checkForErrors(std::source_location location = std::source_location::current()) noexcept override;

        /**
         *
         * @return
         */
        [[nodiscard]] GL46Shader* createShader() override;
        [[nodiscard]] GLVertexArray* createVertexArray() override;
        [[nodiscard]] GLVertexBuffer* createVertexBuffer() override;
        [[nodiscard]] GLVertexBufferLayout* createVertexBufferLayout() override;
        [[nodiscard]] GLIndexBuffer* createIndexBuffer() override;
        [[nodiscard]] GL46Texture2D* createTexture2D() override;
        [[nodiscard]] IUniformBuffer* createUniformBuffer() override;

        [[nodiscard]] ImportedScene::IMesh* createMesh() override;

        static const std::shared_ptr<GL46Renderer>& getInstance() noexcept;
    };
}

#endif //SUNGEARENGINE_GL46RENDERER_H
