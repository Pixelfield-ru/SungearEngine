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
    static GLuint sggDrawModeToGL(const SGDrawMode& sgDrawMode)
    {
        GLuint glDrawMode = GL_TRIANGLES;

        switch(sgDrawMode)
        {
            case SGG_TRIANGLES:
                glDrawMode = GL_TRIANGLES;
                break;
            case SGG_LINES:
                glDrawMode = GL_LINES;
                break;
            case SGG_QUADS:
                glDrawMode = GL_QUADS;
                break;
        }

        return glDrawMode;
    }

    static std::uint32_t sggFaceTypeToGL(const SGFaceType& faceType)
    {
        std::uint32_t glFaceType = GL_BACK;

        switch(faceType)
        {
            case SGG_FRONT_FACE: glFaceType = GL_FRONT; break;
            case SGG_BACK_FACE: glFaceType = GL_BACK; break;
            case SGG_FRONT_BACK_FACE: glFaceType = GL_FRONT_AND_BACK; break;
        }

        return glFaceType;
    }

    static std::uint32_t sggPolygonsOrderToGL(const SGPolygonsOrder& polygonsOrder)
    {
        std::uint32_t glPolygonsOrder = GL_CCW;

        switch(polygonsOrder)
        {
            case SGG_CW: glPolygonsOrder = GL_CW; break;
            case SGG_CCW: glPolygonsOrder = GL_CCW; break;
        }

        return glPolygonsOrder;
    }

    static GLint sggInternalFormatToGL(const SGGColorInternalFormat& sggInternalFormat) noexcept
    {
        switch(sggInternalFormat)
        {
            case SGG_R8: return GL_R8;
            case SGG_R8_SIGNED_NORMALIZED: return GL_R8_SNORM;

            case SGG_R16: return GL_R16;
            case SGG_R16_SIGNED_NORMALIZED: return GL_R16_SNORM;

            case SGG_RG8: return GL_RG8;
            case SGG_RG8_SIGNED_NORMALIZED: return GL_RG8_SNORM;

            case SGG_RG16: return GL_R16;
            case SGG_RG16_SIGNED_NORMALIZED: return GL_R16_SNORM;

            case SGG_R3_G3_B2: return GL_R3_G3_B2;

            case SGG_RGB4: return GL_RGB4;
            case SGG_RGB5: return GL_RGB5;
            case SGG_RGB8: return GL_RGB8;

            case SGG_RGB8_SIGNED_NORMALIZED: return GL_RGB8_SNORM;

            case SGG_RGB10: return GL_RGB10;
            case SGG_RGB12: return GL_RGB12;

            case SGG_RGB16_SIGNED_NORMALIZED: return GL_RGB16_SNORM;

            case SGG_RGBA2: return GL_RGBA2;
            case SGG_RGBA4: return GL_RGBA4;

            case SGG_RGB5_A1: return GL_RGB5_A1;

            case SGG_RGBA8: return GL_RGBA8;
            case SGG_RGBA8_SIGNED_NORMALIZED: return GL_RGBA8_SNORM;

            case SGG_RGB10_A2: return GL_RGB10_A2;
            case SGG_RGB10_A2_UNSIGNED_INT: return GL_RGB10_A2UI;

            case SGG_RGBA12: return GL_RGBA12;
            case SGG_RGBA16: return GL_RGBA16;

            case SGG_SRGB8: return GL_SRGB8;
            case SGG_SRGB8_ALPHA8: return GL_SRGB8_ALPHA8;

            case SGG_R16_FLOAT: return GL_R16F;
            case SGG_RG16_FLOAT: return GL_RG16F;
            case SGG_RGB16_FLOAT: return GL_RGB16F;
            case SGG_RGBA16_FLOAT: return GL_RGBA16F;

            case SGG_R32_FLOAT: return GL_R32F;
            case SGG_RG32_FLOAT: return GL_RG32F;
            case SGG_RGB32_FLOAT: return GL_RGB32F;
            case SGG_RGBA32_FLOAT: return GL_RGBA32F;

            case SGG_R11_G11_B10_FLOAT: return GL_R11F_G11F_B10F;

            case SGG_RGB9_E5: return GL_RGB9_E5;

            case SGG_R8_INT: return GL_R8I;
            case SGG_R8_UNSIGNED_INT: return GL_R8UI;

            case SGG_R16_INT: return GL_R16I;
            case SGG_R16_UNSIGNED_INT: return GL_R16UI;

            case SGG_R32_INT: return GL_R32I;
            case SGG_R32_UNSIGNED_INT: return GL_R32UI;

            case SGG_RG8_INT: return GL_RG8I;
            case SGG_RG8_UNSIGNED_INT: return GL_RG8UI;

            case SGG_RG16_INT: return GL_RG16I;
            case SGG_RG16_UNSIGNED_INT: return GL_RG16UI;

            case SGG_RG32_INT: return GL_RG32I;
            case SGG_RG32_UNSIGNED_INT: return GL_RG32UI;

            case SGG_RGB8_INT: return GL_RGB8I;
            case SGG_RGB8_UNSIGNED_INT: return GL_RGB8UI;

            case SGG_RGB16_INT: return GL_RGB16I;
            case SGG_RGB16_UNSIGNED_INT: return GL_RGB16UI;

            case SGG_RGB32_INT: return GL_RGB32I;
            case SGG_RGB32_UNSIGNED_INT: return GL_RGB32UI;

            case SGG_RGBA8_INT: return GL_RGBA8I;
            case SGG_RGBA8_UNSIGNED_INT: return GL_RGBA8UI;

            case SGG_RGBA16_INT: return GL_RGBA16I;
            case SGG_RGBA16_UNSIGNED_INT: return GL_RGBA16UI;

            case SGG_RGBA32_INT: return GL_RGBA32I;
            case SGG_RGBA32_UNSIGNED_INT: return GL_RGBA32UI;

            case SGG_COMPRESSED_R: return GL_COMPRESSED_RED;
            case SGG_COMPRESSED_RG: return GL_COMPRESSED_RG;
            case SGG_COMPRESSED_RGB: return GL_COMPRESSED_RGB;
            case SGG_COMPRESSED_RGBA: return GL_COMPRESSED_RGBA;

            case SGG_COMPRESSED_SRGB: return GL_COMPRESSED_SRGB;
            case SGG_COMPRESSED_SRGBA: return GL_COMPRESSED_SRGB_ALPHA;

            case SGG_STENCIL_INDEX8: return GL_STENCIL_INDEX8;

            case SGG_DEPTH_COMPONENT16: return GL_DEPTH_COMPONENT16;
            case SGG_DEPTH_COMPONENT24: return GL_DEPTH_COMPONENT24;
            case SGG_DEPTH_COMPONENT32: return GL_DEPTH_COMPONENT32;
            case SGG_DEPTH_COMPONENT32F: return GL_DEPTH_COMPONENT32F;

            case SGG_DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
            case SGG_DEPTH32F_STENCIL8: return GL_DEPTH32F_STENCIL8;

            default: return GL_RGB8;
        }
    }

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
