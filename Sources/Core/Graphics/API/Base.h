//
// Created by stuka on 21.04.2023.
//

#pragma once

#ifndef NATIVECORE_IRENDERER_H
#define NATIVECORE_IRENDERER_H

#include <iostream>

namespace Core::Graphics::API::Base
{
    class IFrameBuffer;
    class IIndexBuffer;
    class ITexture2D;
    class IVertexArray;
    class IVertexBuffer;
    class IRenderer;

    class IFrameBuffer
    {

    };

    class IIndexBuffer
    {

    };

    class ITexture2D
    {

    };

    class IVertexArray
    {

    };

    class IVertexBuffer
    {

    };

    class IRenderer
    {
    public:
        virtual std::uint32_t get_version() = 0;

        virtual void init() = 0;

        virtual void check_for_errors() = 0;

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
