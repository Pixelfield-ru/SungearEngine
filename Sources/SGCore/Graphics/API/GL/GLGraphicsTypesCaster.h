//
// Created by stuka on 28.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GLGRAPHICSTYPESCASTER_H
#define SUNGEARENGINE_GLGRAPHICSTYPESCASTER_H

#include <glad/glad.h>
#include <iostream>
#include "../GraphicsDataTypes.h"

namespace GLGraphicsTypesCaster
{
    static GLint sggInternalFormatToGL(const SGGColorInternalFormat& sggInternalFormat) noexcept
    {
        GLint glInternalFormat = GL_RGB8;

        switch(sggInternalFormat)
        {
            case SGG_R8: glInternalFormat = GL_R8; break;
            case SGG_R8_SIGNED_NORMALIZED: glInternalFormat = GL_R8_SNORM; break;

            case SGG_R16: glInternalFormat = GL_R16; break;
            case SGG_R16_SIGNED_NORMALIZED: glInternalFormat = GL_R16_SNORM; break;

            case SGG_RG8: glInternalFormat = GL_RG8; break;
            case SGG_RG8_SIGNED_NORMALIZED: glInternalFormat = GL_RG8_SNORM; break;

            case SGG_RG16: glInternalFormat = GL_R16; break;
            case SGG_RG16_SIGNED_NORMALIZED: glInternalFormat = GL_R16_SNORM; break;

            case SGG_R3_G3_B2: glInternalFormat = GL_R3_G3_B2; break;

            case SGG_RGB4: glInternalFormat = GL_RGB4; break;
            case SGG_RGB5: glInternalFormat = GL_RGB5; break;
            case SGG_RGB8: glInternalFormat = GL_RGB8; break;

            case SGG_RGB8_SIGNED_NORMALIZED: glInternalFormat = GL_RGB8_SNORM; break;

            case SGG_RGB10: glInternalFormat = GL_RGB10; break;
            case SGG_RGB12: glInternalFormat = GL_RGB12; break;

            case SGG_RGB16_SIGNED_NORMALIZED: glInternalFormat = GL_RGB16_SNORM; break;

            case SGG_RGBA2: glInternalFormat = GL_RGBA2; break;
            case SGG_RGBA4: glInternalFormat = GL_RGBA4; break;

            case SGG_RGB5_A1: glInternalFormat = GL_RGB5_A1; break;

            case SGG_RGBA8: glInternalFormat = GL_RGBA8; break;
            case SGG_RGBA8_SIGNED_NORMALIZED: glInternalFormat = GL_RGBA8_SNORM; break;

            case SGG_RGB10_A2: glInternalFormat = GL_RGB10_A2; break;
            case SGG_RGB10_A2_UNSIGNED_INT: glInternalFormat = GL_RGB10_A2UI; break;

            case SGG_RGBA12: glInternalFormat = GL_RGBA12; break;
            case SGG_RGBA16: glInternalFormat = GL_RGBA16; break;

            case SGG_SRGB8: glInternalFormat = GL_SRGB8; break;
            case SGG_SRGB8_ALPHA8: glInternalFormat = GL_SRGB8_ALPHA8; break;

            case SGG_R16_FLOAT: glInternalFormat = GL_R16F; break;
            case SGG_RG16_FLOAT: glInternalFormat = GL_RG16F; break;
            case SGG_RGB16_FLOAT: glInternalFormat = GL_RGB16F; break;
            case SGG_RGBA16_FLOAT: glInternalFormat = GL_RGBA16F; break;

            case SGG_R32_FLOAT: glInternalFormat = GL_R32F; break;
            case SGG_RG32_FLOAT: glInternalFormat = GL_RG32F; break;
            case SGG_RGB32_FLOAT: glInternalFormat = GL_RGB32F; break;
            case SGG_RGBA32_FLOAT: glInternalFormat = GL_RGBA32F; break;

            case SGG_R11_G11_B10_FLOAT: glInternalFormat = GL_R11F_G11F_B10F; break;

            case SGG_RGB9_E5: glInternalFormat = GL_RGB9_E5; break;

            case SGG_R8_INT: glInternalFormat = GL_R8I; break;
            case SGG_R8_UNSIGNED_INT: glInternalFormat = GL_R8UI; break;

            case SGG_R16_INT: glInternalFormat = GL_R16I; break;
            case SGG_R16_UNSIGNED_INT: glInternalFormat = GL_R16UI; break;

            case SGG_R32_INT: glInternalFormat = GL_R32I; break;
            case SGG_R32_UNSIGNED_INT: glInternalFormat = GL_R32UI; break;

            case SGG_RG8_INT: glInternalFormat = GL_RG8I; break;
            case SGG_RG8_UNSIGNED_INT: glInternalFormat = GL_RG8UI; break;

            case SGG_RG16_INT: glInternalFormat = GL_RG16I; break;
            case SGG_RG16_UNSIGNED_INT: glInternalFormat = GL_RG16UI; break;

            case SGG_RG32_INT: glInternalFormat = GL_RG32I; break;
            case SGG_RG32_UNSIGNED_INT: glInternalFormat = GL_RG32UI; break;

            case SGG_RGB8_INT: glInternalFormat = GL_RGB8I; break;
            case SGG_RGB8_UNSIGNED_INT: glInternalFormat = GL_RGB8UI; break;

            case SGG_RGB16_INT: glInternalFormat = GL_RGB16I; break;
            case SGG_RGB16_UNSIGNED_INT: glInternalFormat = GL_RGB16UI; break;

            case SGG_RGB32_INT: glInternalFormat = GL_RGB32I; break;
            case SGG_RGB32_UNSIGNED_INT: glInternalFormat = GL_RGB32UI; break;

            case SGG_RGBA8_INT: glInternalFormat = GL_RGBA8I; break;
            case SGG_RGBA8_UNSIGNED_INT: glInternalFormat = GL_RGBA8UI; break;

            case SGG_RGBA16_INT: glInternalFormat = GL_RGBA16I; break;
            case SGG_RGBA16_UNSIGNED_INT: glInternalFormat = GL_RGBA16UI; break;

            case SGG_RGBA32_INT: glInternalFormat = GL_RGBA32I; break;
            case SGG_RGBA32_UNSIGNED_INT: glInternalFormat = GL_RGBA32UI; break;

            case SGG_COMPRESSED_R: glInternalFormat = GL_COMPRESSED_RED; break;
            case SGG_COMPRESSED_RG: glInternalFormat = GL_COMPRESSED_RG; break;
            case SGG_COMPRESSED_RGB: glInternalFormat = GL_COMPRESSED_RGB; break;
            case SGG_COMPRESSED_RGBA: glInternalFormat = GL_COMPRESSED_RGBA; break;

            case SGG_COMPRESSED_SRGB: glInternalFormat = GL_COMPRESSED_SRGB; break;
            case SGG_COMPRESSED_SRGBA: glInternalFormat = GL_COMPRESSED_SRGB_ALPHA; break;

            case SGG_STENCIL_INDEX8: glInternalFormat = GL_STENCIL_INDEX8; break;

            case SGG_DEPTH_COMPONENT16: glInternalFormat = GL_DEPTH_COMPONENT16; break;
            case SGG_DEPTH_COMPONENT24: glInternalFormat = GL_DEPTH_COMPONENT24; break;
            case SGG_DEPTH_COMPONENT32: glInternalFormat = GL_DEPTH_COMPONENT32; break;
            case SGG_DEPTH_COMPONENT32F: glInternalFormat = GL_DEPTH_COMPONENT32F; break;

            case SGG_DEPTH24_STENCIL8: glInternalFormat = GL_DEPTH24_STENCIL8; break;
            case SGG_DEPTH32F_STENCIL8: glInternalFormat = GL_DEPTH32F_STENCIL8; break;
        }

        return glInternalFormat;
    };

    static GLenum sggFormatToGL(const SGGColorFormat& sggFormat) noexcept
    {
        GLenum glFormat = GL_RGB;

        switch(sggFormat)
        {
            case SGG_R: glFormat = GL_RED; break;
            case SGG_RG: glFormat = GL_RG; break;
            case SGG_RGB: glFormat = GL_RGB; break;
            case SGG_BGR: glFormat = GL_BGR; break;
            case SGG_RGBA: glFormat = GL_RGBA; break;
            case SGG_BGRA: glFormat = GL_BGRA; break;

            case SGG_R_INTEGER: glFormat = GL_RED_INTEGER; break;
            case SGG_RG_INTEGER: glFormat = GL_RG_INTEGER; break;
            case SGG_RGB_INTEGER: glFormat = GL_RGB_INTEGER; break;
            case SGG_BGR_INTEGER: glFormat = GL_BGR_INTEGER; break;
            case SGG_RGBA_INTEGER: glFormat = GL_RGBA_INTEGER; break;
            case SGG_BGRA_INTEGER: glFormat = GL_BGRA_INTEGER; break;

            case SGG_STENCIL_INDEX: glFormat = GL_STENCIL_INDEX; break;

            case SGG_DEPTH_COMPONENT: glFormat = GL_DEPTH_COMPONENT; break;
            case SGG_DEPTH_STENCIL: glFormat = GL_DEPTH_STENCIL; break;
        }

        return glFormat;
    }

    static GLenum sggBufferUsageToGL(const SGGUsage& sggBufferUsage) noexcept
    {
        GLenum usage = GL_STATIC_DRAW;

        switch(sggBufferUsage)
        {
            case SGG_DYNAMIC: usage = GL_DYNAMIC_DRAW; break;
            case SGG_STATIC: usage = GL_STATIC_DRAW; break;
        }

        return usage;
    }

    static std::uint16_t sggDataTypeToGL(const SGGDataType& sggDataType) noexcept
    {
        int apiDataType;

        switch(sggDataType)
        {
            case SGG_NONE: apiDataType = GL_NONE; break;

            case SGG_INT: apiDataType = GL_INT; break;
            case SGG_INT2: apiDataType = GL_INT; break;
            case SGG_INT3: apiDataType = GL_INT; break;
            case SGG_INT4: apiDataType = GL_INT; break;

            case SGG_FLOAT: apiDataType = GL_FLOAT; break;
            case SGG_FLOAT2: apiDataType = GL_FLOAT; break;
            case SGG_FLOAT3: apiDataType = GL_FLOAT; break;
            case SGG_FLOAT4: apiDataType = GL_FLOAT; break;

            case SGG_MAT2: apiDataType = GL_FLOAT; break;
            case SGG_MAT3: apiDataType = GL_FLOAT; break;
            case SGG_MAT4: apiDataType = GL_FLOAT; break;

            case SGG_BOOL: apiDataType = GL_BOOL; break;

            default: apiDataType = GL_NONE; break;
        }

        return apiDataType;
    }
};

#endif //SUNGEARENGINE_GLGRAPHICSTYPESCASTER_H
