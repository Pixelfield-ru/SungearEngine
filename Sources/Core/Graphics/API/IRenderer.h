//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_IRENDERER_H
#define NATIVECORE_IRENDERER_H

#include <iostream>

#include "../../Main/Window.h"

namespace Core::Graphics::API
{
    class IRenderer
    {
    public:
        virtual void print_info() noexcept { }

        virtual void init(const Main::Window& wnd) { }

        virtual void start_loop() { }

        virtual void check_for_errors() noexcept { }

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
