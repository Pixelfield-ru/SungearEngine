//
// Created by stuka on 28.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GLGRAPHICSTYPESCASTER_H
#define SUNGEARENGINE_GLGRAPHICSTYPESCASTER_H

#include <SGCore/pch.h>

#include "../GraphicsDataTypes.h"

namespace GLGraphicsTypesCaster
{
    static std::uint32_t sggDrawModeToGL(const SGDrawMode& sgDrawMode)
    {
        switch(sgDrawMode)
        {
            case SGDrawMode::SGG_TRIANGLES:
                return GL_TRIANGLES;
            case SGDrawMode::SGG_LINES:
                return GL_LINES;
            case SGDrawMode::SGG_QUADS:
                return GL_QUADS;
            case SGDrawMode::SGG_POINTS:
                return GL_POINTS;
            case SGDrawMode::SGG_TRIANGLE_STRIP:
                return GL_TRIANGLE_STRIP;
                break;
            case SGDrawMode::SGG_TRIANGLE_FAN:
                return GL_TRIANGLE_FAN;
                break;
            
            default:
                return GL_TRIANGLES;
        }
    }

    static std::uint32_t sggFaceTypeToGL(const SGFaceType& faceType)
    {
        switch(faceType)
        {
            case SGFaceType::SGG_FRONT_FACE: return GL_FRONT;
            case SGFaceType::SGG_BACK_FACE: return GL_BACK;
            case SGFaceType::SGG_FRONT_BACK_FACE: return GL_FRONT_AND_BACK;

            default: return GL_BACK;
        }
    }

    static std::uint32_t sggPolygonsOrderToGL(const SGPolygonsOrder& polygonsOrder)
    {
        switch(polygonsOrder)
        {
            case SGPolygonsOrder::SGG_CW: return GL_CW;
            case SGPolygonsOrder::SGG_CCW: return GL_CCW;
        }
    }

    static std::int32_t sggInternalFormatToGL(const SGGColorInternalFormat& sggInternalFormat) noexcept
    {
        switch(sggInternalFormat)
        {
            case SGGColorInternalFormat::SGG_RED: return GL_RED;
            
            case SGGColorInternalFormat::SGG_R8: return GL_R8;
            case SGGColorInternalFormat::SGG_R8_SIGNED_NORMALIZED: return GL_R8_SNORM;

            case SGGColorInternalFormat::SGG_R16: return GL_R16;
            case SGGColorInternalFormat::SGG_R16_SIGNED_NORMALIZED: return GL_R16_SNORM;

            case SGGColorInternalFormat::SGG_RG8: return GL_RG8;
            case SGGColorInternalFormat::SGG_RG8_SIGNED_NORMALIZED: return GL_RG8_SNORM;

            case SGGColorInternalFormat::SGG_RG16: return GL_R16;
            case SGGColorInternalFormat::SGG_RG16_SIGNED_NORMALIZED: return GL_R16_SNORM;

            case SGGColorInternalFormat::SGG_R3_G3_B2: return GL_R3_G3_B2;

            case SGGColorInternalFormat::SGG_RGB4: return GL_RGB4;
            case SGGColorInternalFormat::SGG_RGB5: return GL_RGB5;
            case SGGColorInternalFormat::SGG_RGB8: return GL_RGB8;

            case SGGColorInternalFormat::SGG_RGB8_SIGNED_NORMALIZED: return GL_RGB8_SNORM;

            case SGGColorInternalFormat::SGG_RGB10: return GL_RGB10;
            case SGGColorInternalFormat::SGG_RGB12: return GL_RGB12;

            case SGGColorInternalFormat::SGG_RGB16_SIGNED_NORMALIZED: return GL_RGB16_SNORM;

            case SGGColorInternalFormat::SGG_RGBA2: return GL_RGBA2;
            case SGGColorInternalFormat::SGG_RGBA4: return GL_RGBA4;

            case SGGColorInternalFormat::SGG_RGB5_A1: return GL_RGB5_A1;

            case SGGColorInternalFormat::SGG_RGBA8: return GL_RGBA8;
            case SGGColorInternalFormat::SGG_RGBA8_SIGNED_NORMALIZED: return GL_RGBA8_SNORM;

            case SGGColorInternalFormat::SGG_RGB10_A2: return GL_RGB10_A2;
            case SGGColorInternalFormat::SGG_RGB10_A2_UNSIGNED_INT: return GL_RGB10_A2UI;

            case SGGColorInternalFormat::SGG_RGBA12: return GL_RGBA12;
            case SGGColorInternalFormat::SGG_RGBA16: return GL_RGBA16;

            case SGGColorInternalFormat::SGG_SRGB8: return GL_SRGB8;
            case SGGColorInternalFormat::SGG_SRGB8_ALPHA8: return GL_SRGB8_ALPHA8;

            case SGGColorInternalFormat::SGG_R16_FLOAT: return GL_R16F;
            case SGGColorInternalFormat::SGG_RG16_FLOAT: return GL_RG16F;
            case SGGColorInternalFormat::SGG_RGB16_FLOAT: return GL_RGB16F;
            case SGGColorInternalFormat::SGG_RGBA16_FLOAT: return GL_RGBA16F;

            case SGGColorInternalFormat::SGG_R32_FLOAT: return GL_R32F;
            case SGGColorInternalFormat::SGG_RG32_FLOAT: return GL_RG32F;
            case SGGColorInternalFormat::SGG_RGB32_FLOAT: return GL_RGB32F;
            case SGGColorInternalFormat::SGG_RGBA32_FLOAT: return GL_RGBA32F;

            case SGGColorInternalFormat::SGG_R11_G11_B10_FLOAT: return GL_R11F_G11F_B10F;

            case SGGColorInternalFormat::SGG_RGB9_E5: return GL_RGB9_E5;

            case SGGColorInternalFormat::SGG_R8_INT: return GL_R8I;
            case SGGColorInternalFormat::SGG_R8_UNSIGNED_INT: return GL_R8UI;

            case SGGColorInternalFormat::SGG_R16_INT: return GL_R16I;
            case SGGColorInternalFormat::SGG_R16_UNSIGNED_INT: return GL_R16UI;

            case SGGColorInternalFormat::SGG_R32_INT: return GL_R32I;
            case SGGColorInternalFormat::SGG_R32_UNSIGNED_INT: return GL_R32UI;

            case SGGColorInternalFormat::SGG_RG8_INT: return GL_RG8I;
            case SGGColorInternalFormat::SGG_RG8_UNSIGNED_INT: return GL_RG8UI;

            case SGGColorInternalFormat::SGG_RG16_INT: return GL_RG16I;
            case SGGColorInternalFormat::SGG_RG16_UNSIGNED_INT: return GL_RG16UI;

            case SGGColorInternalFormat::SGG_RG32_INT: return GL_RG32I;
            case SGGColorInternalFormat::SGG_RG32_UNSIGNED_INT: return GL_RG32UI;

            case SGGColorInternalFormat::SGG_RGB8_INT: return GL_RGB8I;
            case SGGColorInternalFormat::SGG_RGB8_UNSIGNED_INT: return GL_RGB8UI;

            case SGGColorInternalFormat::SGG_RGB16_INT: return GL_RGB16I;
            case SGGColorInternalFormat::SGG_RGB16_UNSIGNED_INT: return GL_RGB16UI;

            case SGGColorInternalFormat::SGG_RGB32_INT: return GL_RGB32I;
            case SGGColorInternalFormat::SGG_RGB32_UNSIGNED_INT: return GL_RGB32UI;

            case SGGColorInternalFormat::SGG_RGBA8_INT: return GL_RGBA8I;
            case SGGColorInternalFormat::SGG_RGBA8_UNSIGNED_INT: return GL_RGBA8UI;

            case SGGColorInternalFormat::SGG_RGBA16_INT: return GL_RGBA16I;
            case SGGColorInternalFormat::SGG_RGBA16_UNSIGNED_INT: return GL_RGBA16UI;

            case SGGColorInternalFormat::SGG_RGBA32_INT: return GL_RGBA32I;
            case SGGColorInternalFormat::SGG_RGBA32_UNSIGNED_INT: return GL_RGBA32UI;

            case SGGColorInternalFormat::SGG_COMPRESSED_R: return GL_COMPRESSED_RED;
            case SGGColorInternalFormat::SGG_COMPRESSED_RG: return GL_COMPRESSED_RG;
            case SGGColorInternalFormat::SGG_COMPRESSED_RGB: return GL_COMPRESSED_RGB;
            case SGGColorInternalFormat::SGG_COMPRESSED_RGBA: return GL_COMPRESSED_RGBA;

            case SGGColorInternalFormat::SGG_COMPRESSED_SRGB: return GL_COMPRESSED_SRGB;
            case SGGColorInternalFormat::SGG_COMPRESSED_SRGBA: return GL_COMPRESSED_SRGB_ALPHA;

            case SGGColorInternalFormat::SGG_STENCIL_INDEX8: return GL_STENCIL_INDEX8;

            case SGGColorInternalFormat::SGG_DEPTH_COMPONENT16: return GL_DEPTH_COMPONENT16;
            case SGGColorInternalFormat::SGG_DEPTH_COMPONENT24: return GL_DEPTH_COMPONENT24;
            case SGGColorInternalFormat::SGG_DEPTH_COMPONENT32: return GL_DEPTH_COMPONENT32;
            case SGGColorInternalFormat::SGG_DEPTH_COMPONENT32F: return GL_DEPTH_COMPONENT32F;

            case SGGColorInternalFormat::SGG_DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
            case SGGColorInternalFormat::SGG_DEPTH32F_STENCIL8: return GL_DEPTH32F_STENCIL8;

            default: return GL_RGB8;
        }
    }

    static std::uint32_t sggFormatToGL(const SGGColorFormat& sggFormat) noexcept
    {
        switch(sggFormat)
        {
            case SGGColorFormat::SGG_R: return GL_RED;
            case SGGColorFormat::SGG_RG: return GL_RG;
            case SGGColorFormat::SGG_RGB: return GL_RGB;
            case SGGColorFormat::SGG_BGR: return GL_BGR;
            case SGGColorFormat::SGG_RGBA: return GL_RGBA;
            case SGGColorFormat::SGG_BGRA: return GL_BGRA;

            case SGGColorFormat::SGG_R_INTEGER: return GL_RED_INTEGER;
            case SGGColorFormat::SGG_RG_INTEGER: return GL_RG_INTEGER;
            case SGGColorFormat::SGG_RGB_INTEGER: return GL_RGB_INTEGER;
            case SGGColorFormat::SGG_BGR_INTEGER: return GL_BGR_INTEGER;
            case SGGColorFormat::SGG_RGBA_INTEGER: return GL_RGBA_INTEGER;
            case SGGColorFormat::SGG_BGRA_INTEGER: return GL_BGRA_INTEGER;

            case SGGColorFormat::SGG_STENCIL_INDEX: return GL_STENCIL_INDEX;

            case SGGColorFormat::SGG_DEPTH_COMPONENT: return GL_DEPTH_COMPONENT;
            case SGGColorFormat::SGG_DEPTH_STENCIL: return GL_DEPTH_STENCIL;

            default: return GL_RGB;
        }
    }

    static std::uint32_t sggBufferUsageToGL(const SGGUsage& sggBufferUsage) noexcept
    {
        switch(sggBufferUsage)
        {
            case SGGUsage::SGG_DYNAMIC: return GL_DYNAMIC_DRAW;
            case SGGUsage::SGG_STATIC: return GL_STATIC_DRAW;
        }
    }

    static std::uint32_t sggDataTypeToGL(const SGGDataType& sggDataType) noexcept
    {
        switch(sggDataType)
        {
            case SGGDataType::SGG_NONE: return GL_NONE;

            case SGGDataType::SGG_UNSIGNED_INT: return GL_UNSIGNED_INT;
            case SGGDataType::SGG_INT: return GL_INT;
            case SGGDataType::SGG_INT2: return GL_INT;
            case SGGDataType::SGG_INT3: return GL_INT;
            case SGGDataType::SGG_INT4: return GL_INT;

            case SGGDataType::SGG_FLOAT: return GL_FLOAT;
            case SGGDataType::SGG_FLOAT2: return GL_FLOAT;
            case SGGDataType::SGG_FLOAT3: return GL_FLOAT;
            case SGGDataType::SGG_FLOAT4: return GL_FLOAT;

            case SGGDataType::SGG_MAT2: return GL_FLOAT;
            case SGGDataType::SGG_MAT3: return GL_FLOAT;
            case SGGDataType::SGG_MAT4: return GL_FLOAT;

            case SGGDataType::SGG_UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
            case SGGDataType::SGG_BYTE: return GL_BYTE;

            case SGGDataType::SGG_BOOL: return GL_BOOL;

            default: return GL_NONE;
        }
    }

    static std::uint32_t sgStencilOpToGL(const SGStencilOp& sggDataType) noexcept
    {
        switch(sggDataType)
        {
            case SGStencilOp::SGG_KEEP: return GL_KEEP;
            case SGStencilOp::SGG_REPLACE: return GL_REPLACE;
            case SGStencilOp::SGG_ZERO: return GL_ZERO;
            case SGStencilOp::SGG_INCR: return GL_INCR;
            case SGStencilOp::SGG_INCR_WRAP: return GL_INCR_WRAP;
            case SGStencilOp::SGG_DECR: return GL_DECR;
            case SGStencilOp::SGG_DECR_WRAP: return GL_DECR_WRAP;
            case SGStencilOp::SGG_INVERT: return GL_INVERT;
            default: return GL_NONE;
        }
    }

    static std::uint32_t sgDepthStencilFuncToGL(const SGDepthStencilFunc& sggDataType) noexcept
    {
        switch(sggDataType)
        {
            case SGDepthStencilFunc::SGG_NEVER: return GL_NEVER;
            case SGDepthStencilFunc::SGG_LESS: return GL_LESS;
            case SGDepthStencilFunc::SGG_LEQUAL: return GL_LEQUAL;
            case SGDepthStencilFunc::SGG_GREATER: return GL_GREATER;
            case SGDepthStencilFunc::SGG_GEQUAL: return GL_GEQUAL;
            case SGDepthStencilFunc::SGG_EQUAL: return GL_EQUAL;
            case SGDepthStencilFunc::SGG_NOTEQUAL: return GL_NOTEQUAL;
            case SGDepthStencilFunc::SGG_ALWAYS: return GL_ALWAYS;
            default: return GL_NONE;
        }
    }

    static std::uint32_t sgBlendingFactorToGL(const SGBlendingFactor& sggDataType) noexcept
    {
        switch(sggDataType)
        {
            case SGBlendingFactor::SGG_ZERO: return GL_ZERO;
            case SGBlendingFactor::SGG_ONE: return GL_ONE;
            case SGBlendingFactor::SGG_SRC_COLOR: return GL_SRC_COLOR;
            case SGBlendingFactor::SGG_ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
            case SGBlendingFactor::SGG_DST_COLOR: return GL_DST_COLOR;
            case SGBlendingFactor::SGG_ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
            case SGBlendingFactor::SGG_SRC_ALPHA: return GL_SRC_ALPHA;
            case SGBlendingFactor::SGG_ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
            case SGBlendingFactor::SGG_DST_ALPHA: return GL_DST_ALPHA;
            case SGBlendingFactor::SGG_ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
            case SGBlendingFactor::SGG_CONST_COLOR: return GL_CONSTANT_COLOR;
            case SGBlendingFactor::SGG_ONE_MINUS_CONST_COLOR: return GL_ONE_MINUS_CONSTANT_COLOR;
            case SGBlendingFactor::SGG_CONST_ALPHA: return GL_CONSTANT_ALPHA;
            case SGBlendingFactor::SGG_ONE_MINUS_CONST_ALPHA: return GL_ONE_MINUS_CONSTANT_ALPHA;
            default: return GL_ZERO;
        }
    }

    static std::uint32_t sgEquationToGL(const SGEquation& sggDataType) noexcept
    {
        switch(sggDataType)
        {
            case SGEquation::SGG_FUNC_ADD: return GL_FUNC_ADD;
            case SGEquation::SGG_FUNC_SUBTRACT: return GL_FUNC_SUBTRACT;
            case SGEquation::SGG_FUNC_REVERSE_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT;
            case SGEquation::SGG_MIN: return GL_MIN;
            case SGEquation::SGG_MAX: return GL_MAX;
            default: return GL_FUNC_ADD;
        }
    }

    static SGGColorFormat oglFormatToSG(GLenum glFormat) noexcept
    {
        switch(glFormat)
        {
            case GL_RED: return SGGColorFormat::SGG_R;
            case GL_RG: return SGGColorFormat::SGG_RG;
            case GL_RGB: return SGGColorFormat::SGG_RGB;
            case GL_BGR: return SGGColorFormat::SGG_BGR;
            case GL_RGBA: return SGGColorFormat::SGG_RGBA;
            case GL_BGRA: return SGGColorFormat::SGG_BGRA;

            case GL_RED_INTEGER: return SGGColorFormat::SGG_R_INTEGER;
            case GL_RG_INTEGER: return SGGColorFormat::SGG_RG_INTEGER;
            case GL_RGB_INTEGER: return SGGColorFormat::SGG_RGB_INTEGER;
            case GL_BGR_INTEGER: return SGGColorFormat::SGG_BGR_INTEGER;
            case GL_RGBA_INTEGER: return SGGColorFormat::SGG_RGBA_INTEGER;
            case GL_BGRA_INTEGER: return SGGColorFormat::SGG_BGRA_INTEGER;

            case GL_STENCIL_INDEX: return SGGColorFormat::SGG_STENCIL_INDEX;

            case GL_DEPTH_COMPONENT: return SGGColorFormat::SGG_DEPTH_COMPONENT;
            case GL_DEPTH_STENCIL: return SGGColorFormat::SGG_DEPTH_STENCIL;

            default: return SGGColorFormat::SGG_RGB;
        }
    }

    static SGGColorInternalFormat oglInternalFormatToSG(GLenum glInternalFormat) noexcept
    {
        switch(glInternalFormat)
        {
            case GL_RED: return SGGColorInternalFormat::SGG_RED;

            case GL_R8: return SGGColorInternalFormat::SGG_R8;
            case GL_R8_SNORM: return SGGColorInternalFormat::SGG_R8_SIGNED_NORMALIZED;

            case GL_R16: return SGGColorInternalFormat::SGG_R16;
            case GL_R16_SNORM: return SGGColorInternalFormat::SGG_R16_SIGNED_NORMALIZED;

            case GL_RG8: return SGGColorInternalFormat::SGG_RG8;
            case GL_RG8_SNORM: return SGGColorInternalFormat::SGG_RG8_SIGNED_NORMALIZED;

            case GL_RG16: return SGGColorInternalFormat::SGG_RG16;
            case GL_RG16_SNORM: return SGGColorInternalFormat::SGG_RG16_SIGNED_NORMALIZED;

            case GL_R3_G3_B2: return SGGColorInternalFormat::SGG_R3_G3_B2;

            case GL_RGB4: return SGGColorInternalFormat::SGG_RGB4;
            case GL_RGB5: return SGGColorInternalFormat::SGG_RGB5;
            case GL_RGB8: return SGGColorInternalFormat::SGG_RGB8;

            case GL_RGB8_SNORM: return SGGColorInternalFormat::SGG_RGB8_SIGNED_NORMALIZED;

            case GL_RGB10: return SGGColorInternalFormat::SGG_RGB10;
            case GL_RGB12: return SGGColorInternalFormat::SGG_RGB12;

            case GL_RGB16_SNORM: return SGGColorInternalFormat::SGG_RGB16_SIGNED_NORMALIZED;

            case GL_RGBA2: return SGGColorInternalFormat::SGG_RGBA2;
            case GL_RGBA4: return SGGColorInternalFormat::SGG_RGBA4;

            case GL_RGB5_A1: return SGGColorInternalFormat::SGG_RGB5_A1;

            case GL_RGBA8: return SGGColorInternalFormat::SGG_RGBA8;
            case GL_RGBA8_SNORM: return SGGColorInternalFormat::SGG_RGBA8_SIGNED_NORMALIZED;

            case GL_RGB10_A2: return SGGColorInternalFormat::SGG_RGB10_A2;
            case GL_RGB10_A2UI: return SGGColorInternalFormat::SGG_RGB10_A2_UNSIGNED_INT;

            case GL_RGBA12: return SGGColorInternalFormat::SGG_RGBA12;
            case GL_RGBA16: return SGGColorInternalFormat::SGG_RGBA16;

            case GL_SRGB8: return SGGColorInternalFormat::SGG_SRGB8;
            case GL_SRGB8_ALPHA8: return SGGColorInternalFormat::SGG_SRGB8_ALPHA8;

            case GL_R16F: return SGGColorInternalFormat::SGG_R16_FLOAT;
            case GL_RG16F: return SGGColorInternalFormat::SGG_RG16_FLOAT;
            case GL_RGB16F: return SGGColorInternalFormat::SGG_RGB16_FLOAT;
            case GL_RGBA16F: return SGGColorInternalFormat::SGG_RGBA16_FLOAT;

            case GL_R32F: return SGGColorInternalFormat::SGG_R32_FLOAT;
            case GL_RG32F: return SGGColorInternalFormat::SGG_RG32_FLOAT;
            case GL_RGB32F: return SGGColorInternalFormat::SGG_RGB32_FLOAT;
            case GL_RGBA32F: return SGGColorInternalFormat::SGG_RGBA32_FLOAT;

            case GL_R11F_G11F_B10F: return SGGColorInternalFormat::SGG_R11_G11_B10_FLOAT;

            case GL_RGB9_E5: return SGGColorInternalFormat::SGG_RGB9_E5;

            case GL_R8I: return SGGColorInternalFormat::SGG_R8_INT;
            case GL_R8UI: return SGGColorInternalFormat::SGG_R8_UNSIGNED_INT;

            case GL_R16I: return SGGColorInternalFormat::SGG_R16_INT;
            case GL_R16UI: return SGGColorInternalFormat::SGG_R16_UNSIGNED_INT;

            case GL_R32I: return SGGColorInternalFormat::SGG_R32_INT;
            case GL_R32UI: return SGGColorInternalFormat::SGG_R32_UNSIGNED_INT;

            case GL_RG8I: return SGGColorInternalFormat::SGG_RG8_INT;
            case GL_RG8UI: return SGGColorInternalFormat::SGG_RG8_UNSIGNED_INT;

            case GL_RG16I: return SGGColorInternalFormat::SGG_RG16_INT;
            case GL_RG16UI: return SGGColorInternalFormat::SGG_RG16_UNSIGNED_INT;

            case GL_RG32I: return SGGColorInternalFormat::SGG_RG32_INT;
            case GL_RG32UI: return SGGColorInternalFormat::SGG_RG32_UNSIGNED_INT;

            case GL_RGB8I: return SGGColorInternalFormat::SGG_RGB8_INT;
            case GL_RGB8UI: return SGGColorInternalFormat::SGG_RGB8_UNSIGNED_INT;

            case GL_RGB16I: return SGGColorInternalFormat::SGG_RGB16_INT;
            case GL_RGB16UI: return SGGColorInternalFormat::SGG_RGB16_UNSIGNED_INT;

            case GL_RGB32I: return SGGColorInternalFormat::SGG_RGB32_INT;
            case GL_RGB32UI: return SGGColorInternalFormat::SGG_RGB32_UNSIGNED_INT;

            case GL_RGBA8I: return SGGColorInternalFormat::SGG_RGBA8_INT;
            case GL_RGBA8UI: return SGGColorInternalFormat::SGG_RGBA8_UNSIGNED_INT;

            case GL_RGBA16I: return SGGColorInternalFormat::SGG_RGBA16_INT;
            case GL_RGBA16UI: return SGGColorInternalFormat::SGG_RGBA16_UNSIGNED_INT;

            case GL_RGBA32I: return SGGColorInternalFormat::SGG_RGBA32_INT;
            case GL_RGBA32UI: return SGGColorInternalFormat::SGG_RGBA32_UNSIGNED_INT;

            case GL_COMPRESSED_RED: return SGGColorInternalFormat::SGG_COMPRESSED_R;
            case GL_COMPRESSED_RG: return SGGColorInternalFormat::SGG_COMPRESSED_RG;
            case GL_COMPRESSED_RGB: return SGGColorInternalFormat::SGG_COMPRESSED_RGB;
            case GL_COMPRESSED_RGBA: return SGGColorInternalFormat::SGG_COMPRESSED_RGBA;

            case GL_COMPRESSED_SRGB: return SGGColorInternalFormat::SGG_COMPRESSED_SRGB;
            case GL_COMPRESSED_SRGB_ALPHA: return SGGColorInternalFormat::SGG_COMPRESSED_SRGBA;

            case GL_STENCIL_INDEX8: return SGGColorInternalFormat::SGG_STENCIL_INDEX8;

            case GL_DEPTH_COMPONENT16: return SGGColorInternalFormat::SGG_DEPTH_COMPONENT16;
            case GL_DEPTH_COMPONENT24: return SGGColorInternalFormat::SGG_DEPTH_COMPONENT24;
            case GL_DEPTH_COMPONENT32: return SGGColorInternalFormat::SGG_DEPTH_COMPONENT32;
            case GL_DEPTH_COMPONENT32F: return SGGColorInternalFormat::SGG_DEPTH_COMPONENT32F;

            case GL_DEPTH24_STENCIL8: return SGGColorInternalFormat::SGG_DEPTH24_STENCIL8;
            case GL_DEPTH32F_STENCIL8: return SGGColorInternalFormat::SGG_DEPTH32F_STENCIL8;

            default: return SGGColorInternalFormat::SGG_RGB8;
        }
    }
};

#endif //SUNGEARENGINE_GLGRAPHICSTYPESCASTER_H
