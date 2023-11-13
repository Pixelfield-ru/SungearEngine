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
        switch(sgDrawMode)
        {
            case SGG_TRIANGLES:
                return GL_TRIANGLES;
            case SGG_LINES:
                return GL_LINES;
            case SGG_QUADS:
                return GL_QUADS;
            case SGG_POINTS:
                return GL_POINTS;

            default:
                return GL_TRIANGLES;
        }
    }

    static std::uint32_t sggFaceTypeToGL(const SGFaceType& faceType)
    {
        switch(faceType)
        {
            case SGG_FRONT_FACE: return GL_FRONT;
            case SGG_BACK_FACE: return GL_BACK;
            case SGG_FRONT_BACK_FACE: return GL_FRONT_AND_BACK;

            default: return GL_BACK;
        }
    }

    static std::uint32_t sggPolygonsOrderToGL(const SGPolygonsOrder& polygonsOrder)
    {
        switch(polygonsOrder)
        {
            case SGG_CW: return GL_CW;
            case SGG_CCW: return GL_CCW;
        }
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
        switch(sggFormat)
        {
            case SGG_R: return GL_RED;
            case SGG_RG: return GL_RG;
            case SGG_RGB: return GL_RGB;
            case SGG_BGR: return GL_BGR;
            case SGG_RGBA: return GL_RGBA;
            case SGG_BGRA: return GL_BGRA;

            case SGG_R_INTEGER: return GL_RED_INTEGER;
            case SGG_RG_INTEGER: return GL_RG_INTEGER;
            case SGG_RGB_INTEGER: return GL_RGB_INTEGER;
            case SGG_BGR_INTEGER: return GL_BGR_INTEGER;
            case SGG_RGBA_INTEGER: return GL_RGBA_INTEGER;
            case SGG_BGRA_INTEGER: return GL_BGRA_INTEGER;

            case SGG_STENCIL_INDEX: return GL_STENCIL_INDEX;

            case SGG_DEPTH_COMPONENT: return GL_DEPTH_COMPONENT;
            case SGG_DEPTH_STENCIL: return GL_DEPTH_STENCIL;

            default: return GL_RGB;
        }
    }

    static GLenum sggBufferUsageToGL(const SGGUsage& sggBufferUsage) noexcept
    {
        switch(sggBufferUsage)
        {
            case SGG_DYNAMIC: return GL_DYNAMIC_DRAW;
            case SGG_STATIC: return GL_STATIC_DRAW;
        }
    }

    static std::uint16_t sggDataTypeToGL(const SGGDataType& sggDataType) noexcept
    {
        switch(sggDataType)
        {
            case SGG_NONE: return GL_NONE;

            case SGG_INT: return GL_INT;
            case SGG_INT2: return GL_INT;
            case SGG_INT3: return GL_INT;
            case SGG_INT4: return GL_INT;

            case SGG_FLOAT: return GL_FLOAT;
            case SGG_FLOAT2: return GL_FLOAT;
            case SGG_FLOAT3: return GL_FLOAT;
            case SGG_FLOAT4: return GL_FLOAT;

            case SGG_MAT2: return GL_FLOAT;
            case SGG_MAT3: return GL_FLOAT;
            case SGG_MAT4: return GL_FLOAT;

            case SGG_BOOL: return GL_BOOL;

            default: return GL_NONE;
        }
    }
};

#endif //SUNGEARENGINE_GLGRAPHICSTYPESCASTER_H
