//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_IRENDERER_H
#define NATIVECORE_IRENDERER_H

#include <iostream>

#include "SGCore/Main/Window.h"

#include "IShader.h"
#include "IVertexArray.h"
#include "IVertexBuffer.h"
#include "IVertexBufferLayout.h"
#include "IIndexBuffer.h"

#include <glm/glm.hpp>
#include <glm/common.hpp>

namespace Core::Graphics::API
{
    class IRenderer
    {
    public:
        virtual void init() { }

        virtual void renderFrame(const glm::ivec2& windowSize) { }

        virtual void renderMesh(IShader*, IVertexArray*) { }

        virtual void printInfo() noexcept { }

        virtual void checkForErrors(std::source_location) noexcept { }

        [[nodiscard]] virtual IShader* createShader() = 0;
        [[nodiscard]] virtual IVertexBuffer* createVertexBuffer() = 0;
        [[nodiscard]] virtual IVertexArray* createVertexArray() = 0;
        [[nodiscard]] virtual IVertexBufferLayout* createVertexBufferLayout() = 0;
        [[nodiscard]] virtual IIndexBuffer* createIndexBuffer() = 0;

        /*
        virtual IIndexBuffer* create_texture2d() = 0;

        virtual IVertexArray* create_vao() = 0;
        virtual IVertexBuffer* create_vbo() = 0;
        virtual IIndexBuffer* create_ibo() = 0;

        virtual IFrameBuffer* create_fbo() = 0;

        virtual IFrameBuffer* create_fbo_color_attachment() = 0;
        virtual std::uint32_t create_fbo_depth_attachment() = 0;
        virtual std::uint32_t create_fbo_depth_stencil_attachment() = 0;
         */
    };
}

#endif //NATIVECORE_IRENDERER_H
