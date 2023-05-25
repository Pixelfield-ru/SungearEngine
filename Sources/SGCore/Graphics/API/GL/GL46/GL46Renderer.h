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
#include "../GLVertexArray.h"
#include "../GLVertexBuffer.h"
#include "../GLVertexBufferLayout.h"
#include "../GLIndexBuffer.h"

#include "SGCore/Graphics/API/IRenderer.h"

namespace Core::Graphics::API::GL::GL46
{
    class GL46Renderer : public IRenderer
    {
    private:
        GL46Renderer() noexcept = default;

        //GL46Renderer() noexcept = default;

        static inline std::shared_ptr<GL46Renderer> m_instance;

    public:
        GL46Renderer(const GL46Renderer&) = delete;
        GL46Renderer(GL46Renderer&&) = delete;

        void init() noexcept override;

        void renderFrame(const glm::ivec2& windowSize) override;

        void renderMesh(IShader*, IVertexArray*) override;

        void printInfo() noexcept override;

        void checkForErrors(std::source_location location = std::source_location::current()) noexcept override;

        [[nodiscard]] GL46Shader* createShader() override;
        [[nodiscard]] GLVertexArray* createVertexArray() override;
        [[nodiscard]] GLVertexBuffer* createVertexBuffer() override;
        [[nodiscard]] GLVertexBufferLayout* createVertexBufferLayout() override;
        [[nodiscard]] GLIndexBuffer* createIndexBuffer() override;

        static const std::shared_ptr<GL46Renderer>& getInstance() noexcept;
    };
}

#endif //SUNGEARENGINE_GL46RENDERER_H