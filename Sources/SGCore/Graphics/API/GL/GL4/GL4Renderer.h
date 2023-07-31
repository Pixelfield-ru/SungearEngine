//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GL4RENDERER_H
#define SUNGEARENGINE_GL4RENDERER_H

#include <glad/glad.h>

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
        std::shared_ptr<GL46UniformBuffer> m_modelMatricesBuffer;
        // Buffer for storing matrices of the currently main camera.
        std::shared_ptr<GL46UniformBuffer> m_viewMatricesBuffer;

        GL4Renderer() noexcept = default;

        static inline std::shared_ptr<GL4Renderer> m_instance;

    public:
        GL4Renderer(const GL4Renderer&) = delete;
        GL4Renderer(GL4Renderer&&) = delete;

        void init() noexcept override;

        bool confirmSupport() noexcept override;

        void renderFrame(const glm::ivec2& windowSize) override;

        void renderMesh(const std::shared_ptr<ECS::CameraComponent>& cameraComponent,
                        const std::shared_ptr<ECS::TransformComponent>& transformComponent,
                        const std::shared_ptr<ECS::MeshComponent>& meshComponent) override;

        void renderMesh(const std::shared_ptr<ECS::ShadowsCasterComponent>& shadowsCasterComponent,
                        const std::shared_ptr<ECS::TransformComponent>& transformComponent,
                        const std::shared_ptr<ECS::MeshComponent>& meshComponent) override;

        void printInfo() noexcept override;

        /**
         * Checks for errors in GAPI.
         * @param location - Where the function is called from.
         */
        void checkForErrors(const std::source_location& location = std::source_location::current()) noexcept override;

        // TODO: create docs
        [[nodiscard]] GL46Shader* createShader() override;
        [[nodiscard]] GL46Shader* createPBRShader() override;
        [[nodiscard]] GL46Shader* createOnlyGeometryShader() override;

        [[nodiscard]] GLVertexArray* createVertexArray() override;
        [[nodiscard]] GLVertexBuffer* createVertexBuffer() override;
        [[nodiscard]] GLVertexBufferLayout* createVertexBufferLayout() override;
        [[nodiscard]] GLIndexBuffer* createIndexBuffer() override;
        [[nodiscard]] ITexture2D* createTexture2D() override;
        [[nodiscard]] GL46UniformBuffer* createUniformBuffer() override;
        [[nodiscard]] GL4FrameBuffer* createFrameBuffer() override;

        [[nodiscard]] GL3Mesh* createMesh() override;

        [[nodiscard]] Memory::Assets::IMaterial* createMaterial() override;

        static const std::shared_ptr<GL4Renderer>& getInstance() noexcept;
    };
}

#endif //SUNGEARENGINE_GL4RENDERER_H
