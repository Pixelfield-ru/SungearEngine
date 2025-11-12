//
// Created by stuka on 22.05.2023.
//

#ifndef SUNGEARENGINE_GRAPHICSDATATYPES_H
#define SUNGEARENGINE_GRAPHICSDATATYPES_H

#include <string>
#include <vector>

#include "SGCore/Utils/TypeTraits.h"

enum class SGStencilOp
{
    SGG_KEEP,
    SGG_REPLACE,
    SGG_ZERO,
    SGG_INCR,
    SGG_INCR_WRAP,
    SGG_DECR,
    SGG_DECR_WRAP,
    SGG_INVERT
};

enum class SGDepthStencilFunc
{
    SGG_NEVER,
    SGG_LESS,
    SGG_LEQUAL,
    SGG_GREATER,
    SGG_GEQUAL,
    SGG_EQUAL,
    SGG_NOTEQUAL,
    SGG_ALWAYS
};

enum class SGBlendingFactor
{
    SGG_ZERO,
    SGG_ONE,
    SGG_SRC_COLOR,
    SGG_ONE_MINUS_SRC_COLOR,
    SGG_DST_COLOR,
    SGG_ONE_MINUS_DST_COLOR,
    SGG_SRC_ALPHA,
    SGG_ONE_MINUS_SRC_ALPHA,
    SGG_DST_ALPHA,
    SGG_ONE_MINUS_DST_ALPHA,
    SGG_CONST_COLOR,
    SGG_ONE_MINUS_CONST_COLOR,
    SGG_CONST_ALPHA,
    SGG_ONE_MINUS_CONST_ALPHA
};

enum class SGEquation
{
    SGG_FUNC_ADD,
    SGG_FUNC_SUBTRACT,
    SGG_FUNC_REVERSE_SUBTRACT,
    SGG_MIN,
    SGG_MAX
};

enum class SGShaderDefineType
{
    SGG_MATERIAL_TEXTURES_BLOCK_DEFINE,
    SGG_OTHER_DEFINE
};

enum class SGDrawMode
{
    SGG_TRIANGLES,
    SGG_TRIANGLE_STRIP,
    SGG_TRIANGLE_FAN,
    SGG_LINES,
    SGG_QUADS,
    SGG_POINTS,
    SGG_PATCHES
};

enum class SGFaceType
{
    SGG_FRONT_FACE,
    SGG_BACK_FACE,
    SGG_FRONT_BACK_FACE
};

enum class SGPolygonsOrder
{
    // clockwise
    SGG_CW,
    // counter-clockwise
    SGG_CCW
};

enum class SGFrameBufferAttachmentType
{
    SGG_DEPTH_ATTACHMENT0,
    SGG_DEPTH_ATTACHMENT1,
    SGG_DEPTH_ATTACHMENT2,
    SGG_DEPTH_ATTACHMENT3,
    SGG_DEPTH_ATTACHMENT4,
    SGG_DEPTH_ATTACHMENT5,
    SGG_DEPTH_ATTACHMENT6,
    SGG_DEPTH_ATTACHMENT7,
    SGG_DEPTH_ATTACHMENT8,
    SGG_DEPTH_ATTACHMENT9,

    SGG_DEPTH_STENCIL_ATTACHMENT0,
    SGG_DEPTH_STENCIL_ATTACHMENT1,
    SGG_DEPTH_STENCIL_ATTACHMENT2,
    SGG_DEPTH_STENCIL_ATTACHMENT3,
    SGG_DEPTH_STENCIL_ATTACHMENT4,
    SGG_DEPTH_STENCIL_ATTACHMENT5,
    SGG_DEPTH_STENCIL_ATTACHMENT6,
    SGG_DEPTH_STENCIL_ATTACHMENT7,
    SGG_DEPTH_STENCIL_ATTACHMENT8,
    SGG_DEPTH_STENCIL_ATTACHMENT9,

    SGG_COLOR_ATTACHMENT0,
    SGG_COLOR_ATTACHMENT1,
    SGG_COLOR_ATTACHMENT2,
    SGG_COLOR_ATTACHMENT3,
    SGG_COLOR_ATTACHMENT4,
    SGG_COLOR_ATTACHMENT5,
    SGG_COLOR_ATTACHMENT6,
    SGG_COLOR_ATTACHMENT7,
    SGG_COLOR_ATTACHMENT8,
    SGG_COLOR_ATTACHMENT9,
    SGG_COLOR_ATTACHMENT10,
    SGG_COLOR_ATTACHMENT11,
    SGG_COLOR_ATTACHMENT12,
    SGG_COLOR_ATTACHMENT13,
    SGG_COLOR_ATTACHMENT14,
    SGG_COLOR_ATTACHMENT15,
    SGG_COLOR_ATTACHMENT16,
    SGG_COLOR_ATTACHMENT17,
    SGG_COLOR_ATTACHMENT18,
    SGG_COLOR_ATTACHMENT19,
    SGG_COLOR_ATTACHMENT20,
    SGG_COLOR_ATTACHMENT21,
    SGG_COLOR_ATTACHMENT22,
    SGG_COLOR_ATTACHMENT23,
    SGG_COLOR_ATTACHMENT24,
    SGG_COLOR_ATTACHMENT25,
    SGG_COLOR_ATTACHMENT26,
    SGG_COLOR_ATTACHMENT27,
    SGG_COLOR_ATTACHMENT28,
    SGG_COLOR_ATTACHMENT29,
    SGG_COLOR_ATTACHMENT30,
    SGG_COLOR_ATTACHMENT31,

    SGG_RENDER_ATTACHMENT0,
    SGG_RENDER_ATTACHMENT1,
    SGG_RENDER_ATTACHMENT2,
    SGG_RENDER_ATTACHMENT3,
    SGG_RENDER_ATTACHMENT4,
    SGG_RENDER_ATTACHMENT5,
    SGG_RENDER_ATTACHMENT6,
    SGG_RENDER_ATTACHMENT7,
    SGG_RENDER_ATTACHMENT8,
    SGG_RENDER_ATTACHMENT9,

    SGG_NOT_ATTACHMENT
};

enum class SGTextureType
{
    SG_TEXTURE2D,
    SG_TEXTURE_BUFFER,
    SG_TEXTURE_CUBEMAP
};

static bool isDepthAttachment(const SGFrameBufferAttachmentType& attachmentType) noexcept
{
    return attachmentType >= SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0 &&
           attachmentType <= SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT9;
}

static bool isDepthStencilAttachment(const SGFrameBufferAttachmentType& attachmentType) noexcept
{
    return attachmentType >= SGFrameBufferAttachmentType::SGG_DEPTH_STENCIL_ATTACHMENT0 &&
           attachmentType <= SGFrameBufferAttachmentType::SGG_DEPTH_STENCIL_ATTACHMENT9;
}

static bool isColorAttachment(const SGFrameBufferAttachmentType& attachmentType) noexcept
{
    return attachmentType >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
           attachmentType <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31;
}

static bool isRenderAttachment(const SGFrameBufferAttachmentType& attachmentType) noexcept
{
    return attachmentType >= SGFrameBufferAttachmentType::SGG_RENDER_ATTACHMENT0 &&
           attachmentType <= SGFrameBufferAttachmentType::SGG_RENDER_ATTACHMENT9;
}

static std::string sgFrameBufferAttachmentTypeToString(const SGFrameBufferAttachmentType& attachmentType) noexcept
{
    if(isDepthAttachment(attachmentType))
    {
        return "SGG_DEPTH_ATTACHMENT" +
               std::to_string(std::to_underlying(attachmentType) -
                              std::to_underlying(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0));
    }
    else if(isDepthStencilAttachment(attachmentType))
    {
        return "SGG_DEPTH_STENCIL_ATTACHMENT" +
                std::to_string(std::to_underlying(attachmentType) -
                               std::to_underlying(SGFrameBufferAttachmentType::SGG_DEPTH_STENCIL_ATTACHMENT0));
    }
    else if(isColorAttachment(attachmentType))
    {
        return "SGG_COLOR_ATTACHMENT" +
               std::to_string(std::to_underlying(attachmentType) -
                              std::to_underlying(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0));
    }
    else if(isRenderAttachment(attachmentType))
    {
        return "SGG_RENDER_ATTACHMENT" +
                std::to_string(std::to_underlying(attachmentType) -
                               std::to_underlying(SGFrameBufferAttachmentType::SGG_RENDER_ATTACHMENT0));
    }

    return "UNKNOWN";
}

enum class SGGDataType
{
    SGG_NONE,

    SGG_UNSIGNED_INT,
    SGG_INT,
    SGG_INT2,
    SGG_INT3,
    SGG_INT4,

    SGG_SHORT,
    SGG_UNSIGNED_SHORT,

    SGG_FLOAT,
    SGG_FLOAT2,
    SGG_FLOAT3,
    SGG_FLOAT4,

    SGG_MAT2,
    SGG_MAT3,
    SGG_MAT4,

    SGG_BOOL,

    SGG_UNSIGNED_BYTE,
    SGG_BYTE
};


enum class SGTextureSlot
{
    SGTT_EMISSIVE,
    SGTT_AMBIENT_OCCLUSION,
    SGTT_AMBIENT,
    SGTT_DIFFUSE_ROUGHNESS,
    SGTT_DIFFUSE,
    SGTT_DISPLACEMENT,
    SGTT_HEIGHT,
    SGTT_NORMALS,
    SGTT_BASE_COLOR,
    SGTT_CLEARCOAT,
    SGTT_EMISSION_COLOR,
    SGTT_LIGHTMAP,
    SGTT_METALNESS,
    SGTT_NORMAL_CAMERA,
    SGTT_OPACITY,
    SGTT_REFLECTION,
    SGTT_SHEEN,
    SGTT_SHININESS,
    SGTT_SPECULAR,
    SGTT_TRANSMISSION,
    SGTT_SKYBOX,
    SGTT_NOISE,

    SGTT_SHADOW_MAP2D,

    SGTT_NONE,

    SGTT_COUNT = SGTT_NONE
};

static constexpr std::uint16_t texture_types_count = std::to_underlying(SGTextureSlot::SGTT_COUNT);

template<typename DataType>
requires(std::is_scalar_v<DataType>)
static constexpr SGGDataType getSGDataTypeFromCPPType() noexcept
{
    if constexpr(std::is_same_v<DataType, float>)
    {
        return SGGDataType::SGG_FLOAT;
    }
    else if constexpr(std::is_same_v<DataType, unsigned char>)
    {
        return SGGDataType::SGG_UNSIGNED_BYTE;
    }
    else if constexpr(std::is_same_v<DataType, char>)
    {
        return SGGDataType::SGG_BYTE;
    }
    else if constexpr(std::is_same_v<DataType, unsigned int>)
    {
        return SGGDataType::SGG_UNSIGNED_INT;
    }
    else if constexpr(std::is_same_v<DataType, int>)
    {
        return SGGDataType::SGG_INT;
    }
    else
    {
        static_assert(SGCore::always_false<DataType>::value, "Unknown DataType");
    }

    return SGGDataType::SGG_NONE;
}

static SGTextureSlot sgStandardTextureFromString(const std::string& textureType) noexcept
{
    if(textureType == "SGTT_EMISSIVE")
    {
        return SGTextureSlot::SGTT_EMISSIVE;
    }
    if(textureType == "SGTT_AMBIENT_OCCLUSION")
    {
        return SGTextureSlot::SGTT_AMBIENT_OCCLUSION;
    }
    if(textureType == "SGTT_AMBIENT")
    {
        return SGTextureSlot::SGTT_AMBIENT;
    }
    if(textureType == "SGTT_DIFFUSE_ROUGHNESS")
    {
        return SGTextureSlot::SGTT_DIFFUSE_ROUGHNESS;
    }
    if(textureType == "SGTT_DIFFUSE")
    {
        return SGTextureSlot::SGTT_DIFFUSE;
    }
    if(textureType == "SGTT_DISPLACEMENT")
    {
        return SGTextureSlot::SGTT_DISPLACEMENT;
    }
    if(textureType == "SGTT_HEIGHT")
    {
        return SGTextureSlot::SGTT_HEIGHT;
    }
    if(textureType == "SGTT_NORMALS")
    {
        return SGTextureSlot::SGTT_NORMALS;
    }
    if(textureType == "SGTT_BASE_COLOR")
    {
        return SGTextureSlot::SGTT_BASE_COLOR;
    }
    if(textureType == "SGTT_CLEARCOAT")
    {
        return SGTextureSlot::SGTT_CLEARCOAT;
    }
    if(textureType == "SGTT_EMISSION_COLOR")
    {
        return SGTextureSlot::SGTT_EMISSION_COLOR;
    }
    if(textureType == "SGTT_LIGHTMAP")
    {
        return SGTextureSlot::SGTT_LIGHTMAP;
    }
    if(textureType == "SGTT_METALNESS")
    {
        return SGTextureSlot::SGTT_METALNESS;
    }
    if(textureType == "SGTT_NORMAL_CAMERA")
    {
        return SGTextureSlot::SGTT_NORMAL_CAMERA;
    }
    if(textureType == "SGTT_OPACITY")
    {
        return SGTextureSlot::SGTT_OPACITY;
    }
    if(textureType == "SGTT_REFLECTION")
    {
        return SGTextureSlot::SGTT_REFLECTION;
    }
    if(textureType == "SGTT_SHEEN")
    {
        return SGTextureSlot::SGTT_SHEEN;
    }
    if(textureType == "SGTT_SHININESS")
    {
        return SGTextureSlot::SGTT_SHININESS;
    }
    if(textureType == "SGTT_SPECULAR")
    {
        return SGTextureSlot::SGTT_SPECULAR;
    }
    if(textureType == "SGTT_TRANSMISSION")
    {
        return SGTextureSlot::SGTT_TRANSMISSION;
    }
    if(textureType == "SGTT_SKYBOX")
    {
        return SGTextureSlot::SGTT_SKYBOX;
    }
    if(textureType == "SGTT_NOISE")
    {
        return SGTextureSlot::SGTT_NOISE;
    }
    if(textureType == "SGTT_SHADOW_MAP2D")
    {
        return SGTextureSlot::SGTT_SHADOW_MAP2D;
    }
    if(textureType == "SGTT_NONE")
    {
        return SGTextureSlot::SGTT_NONE;
    }

    return SGTextureSlot::SGTT_NONE;
}

static std::string sgStandardTextureTypeToString(const SGTextureSlot& sgMaterialTextureType) noexcept
{
    switch(sgMaterialTextureType)
    {
        case SGTextureSlot::SGTT_EMISSIVE: return "SGTT_EMISSIVE";
        case SGTextureSlot::SGTT_AMBIENT_OCCLUSION: return "SGTT_AMBIENT_OCCLUSION";
        case SGTextureSlot::SGTT_AMBIENT: return "SGTT_AMBIENT";
        case SGTextureSlot::SGTT_DIFFUSE_ROUGHNESS: return "SGTT_DIFFUSE_ROUGHNESS";
        case SGTextureSlot::SGTT_DIFFUSE: return "SGTT_DIFFUSE";
        case SGTextureSlot::SGTT_DISPLACEMENT: return "SGTT_DISPLACEMENT";
        case SGTextureSlot::SGTT_HEIGHT: return "SGTT_HEIGHT";
        case SGTextureSlot::SGTT_NORMALS: return "SGTT_NORMALS";
        case SGTextureSlot::SGTT_BASE_COLOR: return "SGTT_BASE_COLOR";
        case SGTextureSlot::SGTT_CLEARCOAT: return "SGTT_CLEARCOAT";
        case SGTextureSlot::SGTT_EMISSION_COLOR: return "SGTT_EMISSION_COLOR";
        case SGTextureSlot::SGTT_LIGHTMAP: return "SGTT_LIGHTMAP";
        case SGTextureSlot::SGTT_METALNESS: return "SGTT_METALNESS";
        case SGTextureSlot::SGTT_NORMAL_CAMERA: return "SGTT_NORMAL_CAMERA";
        case SGTextureSlot::SGTT_OPACITY: return "SGTT_OPACITY";
        case SGTextureSlot::SGTT_REFLECTION: return "SGTT_REFLECTION";
        case SGTextureSlot::SGTT_SHEEN: return "SGTT_SHEEN";
        case SGTextureSlot::SGTT_SHININESS: return "SGTT_SHININESS";
        case SGTextureSlot::SGTT_SPECULAR: return "SGTT_SPECULAR";
        case SGTextureSlot::SGTT_TRANSMISSION: return "SGTT_TRANSMISSION";
        case SGTextureSlot::SGTT_SKYBOX: return "SGTT_SKYBOX";
        case SGTextureSlot::SGTT_NOISE: return "SGTT_NOISE";

        case SGTextureSlot::SGTT_SHADOW_MAP2D: return "SGTT_SHADOW_MAP2D";

        case SGTextureSlot::SGTT_NONE: return "SGTT_NONE";
    }

    return "";
}

static const std::string& sgStandardTextureTypeNameToStandardUniformName(const SGTextureSlot& sgMaterialTextureType) noexcept
{
    switch(sgMaterialTextureType)
    {
        case SGTextureSlot::SGTT_EMISSIVE:
        {
            static std::string name = "mat_emissiveSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_AMBIENT_OCCLUSION:
        {
            static std::string name = "mat_ambientOcclusionSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_AMBIENT:
        {
            static std::string name = "mat_ambientSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_DIFFUSE_ROUGHNESS:
        {
            static std::string name = "mat_diffuseRoughnessSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_DIFFUSE:
        {
            static std::string name = "mat_diffuseSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_DISPLACEMENT:
        {
            static std::string name = "mat_displacementSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_HEIGHT:
        {
            static std::string name = "mat_heightSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_NORMALS:
        {
            static std::string name = "mat_normalsSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_BASE_COLOR:
        {
            static std::string name = "mat_baseColorSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_CLEARCOAT:
        {
            static std::string name = "mat_clearCoatSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_EMISSION_COLOR:
        {
            static std::string name = "mat_emissionColorSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_LIGHTMAP:
        {
            static std::string name = "mat_lightmapSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_METALNESS:
        {
            static std::string name = "mat_metalnessSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_NORMAL_CAMERA:
        {
            static std::string name = "mat_normalCameraSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_OPACITY:
        {
            static std::string name = "mat_opacitySamplers";
            return name;
        }
        case SGTextureSlot::SGTT_REFLECTION:
        {
            static std::string name = "mat_reflectionSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_SHEEN:
        {
            static std::string name = "mat_sheenSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_SHININESS:
        {
            static std::string name = "mat_shininessSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_SPECULAR:
        {
            static std::string name = "mat_specularSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_TRANSMISSION:
        {
            static std::string name = "mat_transmissionSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_SKYBOX:
        {
            static std::string name = "mat_skyboxSamplers";
            return name;
        }
        case SGTextureSlot::SGTT_NOISE:
        {
            static std::string name = "mat_noiseSamplers";
            return name;
        }
        
        case SGTextureSlot::SGTT_SHADOW_MAP2D:
        {
            static std::string name = "mat_shadowMap2DSamplers";
            return name;
        }
        
        case SGTextureSlot::SGTT_NONE:
        {
            static std::string name = "noneSamplers";
            return name;
        }
    }

    static std::string name = "noneSamplers";

    return name;
}

static std::uint16_t getSGGDataTypeSizeInBytes(const SGGDataType& dataType) noexcept
{
    int size;

    switch(dataType)
    {
        case SGGDataType::SGG_NONE: size = 0; break;
        
        case SGGDataType::SGG_UNSIGNED_INT: case SGGDataType::SGG_INT: size = 4; break;
        case SGGDataType::SGG_INT2: size = 4 * 2; break;
        case SGGDataType::SGG_INT3: size = 4 * 3; break;
        case SGGDataType::SGG_INT4: size = 4 * 4; break;

        case SGGDataType::SGG_FLOAT: size = 4; break;
        case SGGDataType::SGG_FLOAT2: size = 4 * 2; break;
        case SGGDataType::SGG_FLOAT3: size = 4 * 3; break;
        case SGGDataType::SGG_FLOAT4: size = 4 * 4; break;

        case SGGDataType::SGG_MAT2: size = 4 * 2 * 2; break;
        case SGGDataType::SGG_MAT3: size = 4 * 3 * 3; break;
        case SGGDataType::SGG_MAT4: size = 4 * 4 * 4; break;

        case SGGDataType::SGG_UNSIGNED_BYTE: case SGGDataType::SGG_BYTE: size = 1; break;
        
        case SGGDataType::SGG_BOOL: size = 1; break;

        default: size = 0; break;
    }

    return size;
}

static std::uint16_t getSGGDataTypeAlignedSizeInBytes(const SGGDataType& dataType) noexcept
{
    int size;

    switch(dataType)
    {
        case SGGDataType::SGG_NONE: size = 0; break;

        case SGGDataType::SGG_INT: size = 4; break;
        case SGGDataType::SGG_INT2: size = 4 * 2; break;
        // 16
        case SGGDataType::SGG_INT3: size = 4 * 3; break;
        case SGGDataType::SGG_INT4: size = 4 * 4; break;

        case SGGDataType::SGG_FLOAT: size = 4; break;
        case SGGDataType::SGG_FLOAT2: size = 4 * 2; break;
        case SGGDataType::SGG_FLOAT3: size = 4 * 3; break;
        case SGGDataType::SGG_FLOAT4: size = 4 * 4; break;

        // 32
        case SGGDataType::SGG_MAT2: size = 4 * 2 * 2; break;
        // 64
        case SGGDataType::SGG_MAT3: size = 4 * 3 * 3; break;
        // 64
        case SGGDataType::SGG_MAT4: size = 4 * 4 * 4; break;

        case SGGDataType::SGG_BOOL: size = 4; break;

        default: size = 0; break;
    }

    return size;
}

enum class SGGUsage
{
    SGG_DYNAMIC,
    SGG_STATIC
};

enum class SGGColorInternalFormat
{
    SGG_R8,
    SGG_R8_SIGNED_NORMALIZED,

    SGG_R16,
    SGG_R16_SIGNED_NORMALIZED,

    SGG_RG8,
    SGG_RG8_SIGNED_NORMALIZED,

    SGG_RG16,
    SGG_RG16_SIGNED_NORMALIZED,

    SGG_R3_G3_B2,

    SGG_RGB4,
    SGG_RGB5,
    SGG_RGB8,

    SGG_RGB8_SIGNED_NORMALIZED,

    SGG_RGB10,
    SGG_RGB12,

    SGG_RGB16_SIGNED_NORMALIZED,

    SGG_RGBA2,
    SGG_RGBA4,

    SGG_RGB5_A1,

    SGG_RGBA8,
    SGG_RGBA8_SIGNED_NORMALIZED,

    SGG_RGB10_A2,
    SGG_RGB10_A2_UNSIGNED_INT,

    SGG_RGBA12,
    SGG_RGBA16,

    SGG_SRGB8,
    SGG_SRGB8_ALPHA8,

    SGG_R16_FLOAT,
    SGG_RG16_FLOAT,
    SGG_RGB16_FLOAT,
    SGG_RGBA16_FLOAT,

    SGG_R32_FLOAT,
    SGG_RG32_FLOAT,
    SGG_RGB32_FLOAT,
    SGG_RGBA32_FLOAT,

    SGG_R11_G11_B10_FLOAT,

    SGG_RGB9_E5, // SHARED BITS

    SGG_R8_INT,
    SGG_R8_UNSIGNED_INT,

    SGG_R16_INT,
    SGG_R16_UNSIGNED_INT,

    SGG_R32_INT,
    SGG_R32_UNSIGNED_INT,

    SGG_RG8_INT,
    SGG_RG8_UNSIGNED_INT,

    SGG_RG16_INT,
    SGG_RG16_UNSIGNED_INT,

    SGG_RG32_INT,
    SGG_RG32_UNSIGNED_INT,

    SGG_RGB8_INT,
    SGG_RGB8_UNSIGNED_INT,

    SGG_RGB16_INT,
    SGG_RGB16_UNSIGNED_INT,

    SGG_RGB32_INT,
    SGG_RGB32_UNSIGNED_INT,

    SGG_RGBA8_INT,
    SGG_RGBA8_UNSIGNED_INT,

    SGG_RGBA16_INT,
    SGG_RGBA16_UNSIGNED_INT,

    SGG_RGBA32_INT,
    SGG_RGBA32_UNSIGNED_INT,

    // compressed
    SGG_COMPRESSED_R,
    SGG_COMPRESSED_RG,
    SGG_COMPRESSED_RGB,
    SGG_COMPRESSED_RGBA,

    SGG_COMPRESSED_SRGB,
    SGG_COMPRESSED_SRGBA,
    // ------------------------
    SGG_STENCIL_INDEX8,

    SGG_DEPTH_COMPONENT16,
    SGG_DEPTH_COMPONENT24,
    SGG_DEPTH_COMPONENT32,
    SGG_DEPTH_COMPONENT32F,

    SGG_DEPTH24_STENCIL8,
    SGG_DEPTH32F_STENCIL8
};

static std::int8_t getSGGInternalFormatChannelsCount(const SGGColorInternalFormat& sggInternalFormat) noexcept
{
    switch(sggInternalFormat)
    {
        case SGGColorInternalFormat::SGG_R8: return 1;
        case SGGColorInternalFormat::SGG_R8_SIGNED_NORMALIZED: return 1;

        case SGGColorInternalFormat::SGG_R16: return 1;
        case SGGColorInternalFormat::SGG_R16_SIGNED_NORMALIZED: return 1;

        case SGGColorInternalFormat::SGG_RG8: return 2;
        case SGGColorInternalFormat::SGG_RG8_SIGNED_NORMALIZED: return 2;

        case SGGColorInternalFormat::SGG_RG16: return 2;
        case SGGColorInternalFormat::SGG_RG16_SIGNED_NORMALIZED: return 2;

        case SGGColorInternalFormat::SGG_R3_G3_B2: return 3;

        case SGGColorInternalFormat::SGG_RGB4: return 3;
        case SGGColorInternalFormat::SGG_RGB5: return 3;
        case SGGColorInternalFormat::SGG_RGB8: return 3;

        case SGGColorInternalFormat::SGG_RGB8_SIGNED_NORMALIZED: return 3;

        case SGGColorInternalFormat::SGG_RGB10: return 3;
        case SGGColorInternalFormat::SGG_RGB12: return 3;

        case SGGColorInternalFormat::SGG_RGB16_SIGNED_NORMALIZED: return 3;

        case SGGColorInternalFormat::SGG_RGBA2: return 4;
        case SGGColorInternalFormat::SGG_RGBA4: return 4;

        case SGGColorInternalFormat::SGG_RGB5_A1: return 4;

        case SGGColorInternalFormat::SGG_RGBA8: return 4;
        case SGGColorInternalFormat::SGG_RGBA8_SIGNED_NORMALIZED: return 4;

        case SGGColorInternalFormat::SGG_RGB10_A2: return 4;
        case SGGColorInternalFormat::SGG_RGB10_A2_UNSIGNED_INT: return 4;

        case SGGColorInternalFormat::SGG_RGBA12: return 4;
        case SGGColorInternalFormat::SGG_RGBA16: return 4;

        case SGGColorInternalFormat::SGG_SRGB8: return 3;
        case SGGColorInternalFormat::SGG_SRGB8_ALPHA8: return 4;

        case SGGColorInternalFormat::SGG_R16_FLOAT: return 1;
        case SGGColorInternalFormat::SGG_RG16_FLOAT: return 2;
        case SGGColorInternalFormat::SGG_RGB16_FLOAT: return 3;
        case SGGColorInternalFormat::SGG_RGBA16_FLOAT: return 4;

        case SGGColorInternalFormat::SGG_R32_FLOAT: return 1;
        case SGGColorInternalFormat::SGG_RG32_FLOAT: return 2;
        case SGGColorInternalFormat::SGG_RGB32_FLOAT: return 3;
        case SGGColorInternalFormat::SGG_RGBA32_FLOAT: return 4;

        case SGGColorInternalFormat::SGG_R11_G11_B10_FLOAT: return 3;

        case SGGColorInternalFormat::SGG_RGB9_E5: return 3;

        case SGGColorInternalFormat::SGG_R8_INT: return 1;
        case SGGColorInternalFormat::SGG_R8_UNSIGNED_INT: return 1;

        case SGGColorInternalFormat::SGG_R16_INT: return 1;
        case SGGColorInternalFormat::SGG_R16_UNSIGNED_INT: return 1;

        case SGGColorInternalFormat::SGG_R32_INT: return 1;
        case SGGColorInternalFormat::SGG_R32_UNSIGNED_INT: return 1;

        case SGGColorInternalFormat::SGG_RG8_INT: return 2;
        case SGGColorInternalFormat::SGG_RG8_UNSIGNED_INT: return 2;

        case SGGColorInternalFormat::SGG_RG16_INT: return 2;
        case SGGColorInternalFormat::SGG_RG16_UNSIGNED_INT: return 2;

        case SGGColorInternalFormat::SGG_RG32_INT: return 2;
        case SGGColorInternalFormat::SGG_RG32_UNSIGNED_INT: return 2;

        case SGGColorInternalFormat::SGG_RGB8_INT: return 3;
        case SGGColorInternalFormat::SGG_RGB8_UNSIGNED_INT: return 3;

        case SGGColorInternalFormat::SGG_RGB16_INT: return 3;
        case SGGColorInternalFormat::SGG_RGB16_UNSIGNED_INT: return 3;

        case SGGColorInternalFormat::SGG_RGB32_INT: return 3;
        case SGGColorInternalFormat::SGG_RGB32_UNSIGNED_INT: return 3;

        case SGGColorInternalFormat::SGG_RGBA8_INT: return 4;
        case SGGColorInternalFormat::SGG_RGBA8_UNSIGNED_INT: return 4;

        case SGGColorInternalFormat::SGG_RGBA16_INT: return 4;
        case SGGColorInternalFormat::SGG_RGBA16_UNSIGNED_INT: return 4;

        case SGGColorInternalFormat::SGG_RGBA32_INT: return 4;
        case SGGColorInternalFormat::SGG_RGBA32_UNSIGNED_INT: return 4;

        case SGGColorInternalFormat::SGG_COMPRESSED_R: return 1;
        case SGGColorInternalFormat::SGG_COMPRESSED_RG: return 2;
        case SGGColorInternalFormat::SGG_COMPRESSED_RGB: return 3;
        case SGGColorInternalFormat::SGG_COMPRESSED_RGBA: return 4;

        case SGGColorInternalFormat::SGG_COMPRESSED_SRGB: return 3;
        case SGGColorInternalFormat::SGG_COMPRESSED_SRGBA: return 4;

        case SGGColorInternalFormat::SGG_STENCIL_INDEX8: return 1;

        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT16: return 1;
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT24: return 1;
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT32: return 1;
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT32F: return 1;

        case SGGColorInternalFormat::SGG_DEPTH24_STENCIL8: return 2;
        case SGGColorInternalFormat::SGG_DEPTH32F_STENCIL8: return 2;

        default: return -1;
    }
}

static constexpr std::uint8_t getSGGInternalFormatChannelsSizeInBytes(SGGColorInternalFormat format) noexcept
{
    switch(format)
    {
        case SGGColorInternalFormat::SGG_R8: return 1;
        case SGGColorInternalFormat::SGG_R8_SIGNED_NORMALIZED: return 1;
        case SGGColorInternalFormat::SGG_R16: return 2;
        case SGGColorInternalFormat::SGG_R16_SIGNED_NORMALIZED: return 2;
        case SGGColorInternalFormat::SGG_RG8: return 2;
        case SGGColorInternalFormat::SGG_RG8_SIGNED_NORMALIZED: return 2;
        case SGGColorInternalFormat::SGG_RG16: return 4;
        case SGGColorInternalFormat::SGG_RG16_SIGNED_NORMALIZED: return 4;
        case SGGColorInternalFormat::SGG_R3_G3_B2: return 1;
        case SGGColorInternalFormat::SGG_RGB4: return 2;
        case SGGColorInternalFormat::SGG_RGB5: return 2;
        case SGGColorInternalFormat::SGG_RGB8: return 3;
        case SGGColorInternalFormat::SGG_RGB8_SIGNED_NORMALIZED: return 3;
        case SGGColorInternalFormat::SGG_RGB10: return 4;
        case SGGColorInternalFormat::SGG_RGB12: return 5;
        case SGGColorInternalFormat::SGG_RGB16_SIGNED_NORMALIZED: return 6;
        case SGGColorInternalFormat::SGG_RGBA2: return 1;
        case SGGColorInternalFormat::SGG_RGBA4: return 2;
        case SGGColorInternalFormat::SGG_RGB5_A1: return 2;
        case SGGColorInternalFormat::SGG_RGBA8: return 4;
        case SGGColorInternalFormat::SGG_RGBA8_SIGNED_NORMALIZED: return 4;
        case SGGColorInternalFormat::SGG_RGB10_A2: return 4;
        case SGGColorInternalFormat::SGG_RGB10_A2_UNSIGNED_INT: return 4;
        case SGGColorInternalFormat::SGG_RGBA12: return 6;
        case SGGColorInternalFormat::SGG_RGBA16: return 8;
        case SGGColorInternalFormat::SGG_SRGB8: return 3;
        case SGGColorInternalFormat::SGG_SRGB8_ALPHA8: return 4;
        case SGGColorInternalFormat::SGG_R16_FLOAT: return 2;
        case SGGColorInternalFormat::SGG_RG16_FLOAT: return 4;
        case SGGColorInternalFormat::SGG_RGB16_FLOAT: return 6;
        case SGGColorInternalFormat::SGG_RGBA16_FLOAT: return 8;
        case SGGColorInternalFormat::SGG_R32_FLOAT: return 4;
        case SGGColorInternalFormat::SGG_RG32_FLOAT: return 8;
        case SGGColorInternalFormat::SGG_RGB32_FLOAT: return 12;
        case SGGColorInternalFormat::SGG_RGBA32_FLOAT: return 16;
        case SGGColorInternalFormat::SGG_R11_G11_B10_FLOAT: return 4;
        case SGGColorInternalFormat::SGG_RGB9_E5: return 4;
        case SGGColorInternalFormat::SGG_R8_INT: return 1;
        case SGGColorInternalFormat::SGG_R8_UNSIGNED_INT: return 1;
        case SGGColorInternalFormat::SGG_R16_INT: return 2;
        case SGGColorInternalFormat::SGG_R16_UNSIGNED_INT: return 2;
        case SGGColorInternalFormat::SGG_R32_INT: return 4;
        case SGGColorInternalFormat::SGG_R32_UNSIGNED_INT: return 4;
        case SGGColorInternalFormat::SGG_RG8_INT: return 2;
        case SGGColorInternalFormat::SGG_RG8_UNSIGNED_INT: return 2;
        case SGGColorInternalFormat::SGG_RG16_INT: return 4;
        case SGGColorInternalFormat::SGG_RG16_UNSIGNED_INT: return 4;
        case SGGColorInternalFormat::SGG_RG32_INT: return 8;
        case SGGColorInternalFormat::SGG_RG32_UNSIGNED_INT: return 8;
        case SGGColorInternalFormat::SGG_RGB8_INT: return 3;
        case SGGColorInternalFormat::SGG_RGB8_UNSIGNED_INT: return 3;
        case SGGColorInternalFormat::SGG_RGB16_INT: return 6;
        case SGGColorInternalFormat::SGG_RGB16_UNSIGNED_INT: return 16;
        case SGGColorInternalFormat::SGG_RGB32_INT: return 12;
        case SGGColorInternalFormat::SGG_RGB32_UNSIGNED_INT: return 12;
        case SGGColorInternalFormat::SGG_RGBA8_INT: return 4;
        case SGGColorInternalFormat::SGG_RGBA8_UNSIGNED_INT: return 4;
        case SGGColorInternalFormat::SGG_RGBA16_INT: return 8;
        case SGGColorInternalFormat::SGG_RGBA16_UNSIGNED_INT: return 8;
        case SGGColorInternalFormat::SGG_RGBA32_INT: return 16;
        case SGGColorInternalFormat::SGG_RGBA32_UNSIGNED_INT: return 16;
        case SGGColorInternalFormat::SGG_COMPRESSED_R: return 0;
        case SGGColorInternalFormat::SGG_COMPRESSED_RG: return 0;
        case SGGColorInternalFormat::SGG_COMPRESSED_RGB: return 0;
        case SGGColorInternalFormat::SGG_COMPRESSED_RGBA: return 0;
        case SGGColorInternalFormat::SGG_COMPRESSED_SRGB: return 0;
        case SGGColorInternalFormat::SGG_COMPRESSED_SRGBA: return 0;
        case SGGColorInternalFormat::SGG_STENCIL_INDEX8: return 1;
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT16: return 2;
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT24: return 3;
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT32: return 4;
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT32F: return 4;
        case SGGColorInternalFormat::SGG_DEPTH24_STENCIL8: return 4;
        case SGGColorInternalFormat::SGG_DEPTH32F_STENCIL8: return 5;
    }

    return 0;
}

enum class SGGColorFormat
{
    SGG_R,
    SGG_RG,
    SGG_RGB,
    SGG_BGR,
    SGG_RGBA,
    SGG_BGRA,

    SGG_R_INTEGER,
    SGG_RG_INTEGER,
    SGG_RGB_INTEGER,
    SGG_BGR_INTEGER,
    SGG_RGBA_INTEGER,
    SGG_BGRA_INTEGER,

    SGG_STENCIL_INDEX,

    SGG_DEPTH_COMPONENT,

    SGG_DEPTH_STENCIL,
    
    SGG_COLOR_FORMAT_NONE
};

enum class SGGChannelType
{
    SGG_R,
    SGG_G,
    SGG_B,
    SGG_A
};

static std::int8_t getSGGFormatChannelsCount(const SGGColorFormat& sggFormat) noexcept
{
    switch(sggFormat)
    {
        case SGGColorFormat::SGG_R: return 1;
        case SGGColorFormat::SGG_RG: return 2;
        case SGGColorFormat::SGG_RGB: return 3;
        case SGGColorFormat::SGG_BGR: return 3;
        case SGGColorFormat::SGG_RGBA: return 4;
        case SGGColorFormat::SGG_BGRA: return 4;

        case SGGColorFormat::SGG_R_INTEGER: return 1;
        case SGGColorFormat::SGG_RG_INTEGER: return 2;
        case SGGColorFormat::SGG_RGB_INTEGER: return 3;
        case SGGColorFormat::SGG_BGR_INTEGER: return 3;
        case SGGColorFormat::SGG_RGBA_INTEGER: return 4;
        case SGGColorFormat::SGG_BGRA_INTEGER: return 4;

        case SGGColorFormat::SGG_STENCIL_INDEX: return 1;

        case SGGColorFormat::SGG_DEPTH_COMPONENT: return 1;
        case SGGColorFormat::SGG_DEPTH_STENCIL: return 2;

        default: return -1;
    }
}

/**
 * @param colorInternalFormat Input internal format of texture.
 * @return Size of every channel in bits in order of color format.
 */
static std::vector<std::uint8_t> getSGGEveryChannelSizeInBits(SGGColorInternalFormat colorInternalFormat) noexcept
{
    switch(colorInternalFormat)
    {
        case SGGColorInternalFormat::SGG_R8: return { 8 };
        case SGGColorInternalFormat::SGG_R8_SIGNED_NORMALIZED: return { 8 };
        case SGGColorInternalFormat::SGG_R16: return { 16 };
        case SGGColorInternalFormat::SGG_R16_SIGNED_NORMALIZED: return { 16 };
        case SGGColorInternalFormat::SGG_RG8: return { 8, 8 };
        case SGGColorInternalFormat::SGG_RG8_SIGNED_NORMALIZED: return { 8, 8 };
        case SGGColorInternalFormat::SGG_RG16: return { 16, 16 };
        case SGGColorInternalFormat::SGG_RG16_SIGNED_NORMALIZED: return { 16, 16 };
        case SGGColorInternalFormat::SGG_R3_G3_B2: return { 3, 3, 2 };
        case SGGColorInternalFormat::SGG_RGB4: return { 4, 4, 4 };
        case SGGColorInternalFormat::SGG_RGB5: return { 5, 5, 5 };
        case SGGColorInternalFormat::SGG_RGB8: return { 8, 8, 8 };
        case SGGColorInternalFormat::SGG_RGB8_SIGNED_NORMALIZED: return { 8, 8, 8 };
        case SGGColorInternalFormat::SGG_RGB10: return { 10, 10, 10 };
        case SGGColorInternalFormat::SGG_RGB12: return { 12, 12, 12 };
        case SGGColorInternalFormat::SGG_RGB16_SIGNED_NORMALIZED: return { 16, 16, 16 };
        case SGGColorInternalFormat::SGG_RGBA2: return { 2, 2, 2, 2 };
        case SGGColorInternalFormat::SGG_RGBA4: return { 4, 4, 4, 4 };
        case SGGColorInternalFormat::SGG_RGB5_A1: return { 5, 5, 5, 1 };
        case SGGColorInternalFormat::SGG_RGBA8: return { 8, 8, 8, 8 };
        case SGGColorInternalFormat::SGG_RGBA8_SIGNED_NORMALIZED: return { 8, 8, 8, 8 };
        case SGGColorInternalFormat::SGG_RGB10_A2: return { 10, 10, 10, 2 };
        case SGGColorInternalFormat::SGG_RGB10_A2_UNSIGNED_INT: return { 10, 10, 10, 2 };
        case SGGColorInternalFormat::SGG_RGBA12: return { 12, 12, 12, 12 };
        case SGGColorInternalFormat::SGG_RGBA16: return { 16, 16, 16, 16 };
        case SGGColorInternalFormat::SGG_SRGB8: return { 8, 8, 8 };
        case SGGColorInternalFormat::SGG_SRGB8_ALPHA8: return { 8, 8, 8, 8 };
        case SGGColorInternalFormat::SGG_R16_FLOAT: return { 16 };
        case SGGColorInternalFormat::SGG_RG16_FLOAT: return { 16, 16 };
        case SGGColorInternalFormat::SGG_RGB16_FLOAT: return { 16, 16, 16 };
        case SGGColorInternalFormat::SGG_RGBA16_FLOAT: return { 16, 16, 16, 16 };
        case SGGColorInternalFormat::SGG_R32_FLOAT: return { 32 };
        case SGGColorInternalFormat::SGG_RG32_FLOAT: return { 32, 32 };
        case SGGColorInternalFormat::SGG_RGB32_FLOAT: return { 32, 32, 32 };
        case SGGColorInternalFormat::SGG_RGBA32_FLOAT: return { 32, 32, 32, 32 };
        case SGGColorInternalFormat::SGG_R11_G11_B10_FLOAT: return { 11, 11, 10 };
        case SGGColorInternalFormat::SGG_RGB9_E5: return { 9, 9, 9, 5 };
        case SGGColorInternalFormat::SGG_R8_INT: return { 8 };
        case SGGColorInternalFormat::SGG_R8_UNSIGNED_INT: return { 8 };
        case SGGColorInternalFormat::SGG_R16_INT: return { 16 };
        case SGGColorInternalFormat::SGG_R16_UNSIGNED_INT: return { 16 };
        case SGGColorInternalFormat::SGG_R32_INT: return { 32 };
        case SGGColorInternalFormat::SGG_R32_UNSIGNED_INT: return { 32 };
        case SGGColorInternalFormat::SGG_RG8_INT: return { 8, 8 };
        case SGGColorInternalFormat::SGG_RG8_UNSIGNED_INT: return { 8, 8 };
        case SGGColorInternalFormat::SGG_RG16_INT: return { 16, 16 };
        case SGGColorInternalFormat::SGG_RG16_UNSIGNED_INT: return { 16, 16 };
        case SGGColorInternalFormat::SGG_RG32_INT: return { 32, 32 };
        case SGGColorInternalFormat::SGG_RG32_UNSIGNED_INT: return { 32, 32 };
        case SGGColorInternalFormat::SGG_RGB8_INT: return { 8, 8, 8 };
        case SGGColorInternalFormat::SGG_RGB8_UNSIGNED_INT: return { 8, 8, 8 };
        case SGGColorInternalFormat::SGG_RGB16_INT: return { 16, 16, 16 };
        case SGGColorInternalFormat::SGG_RGB16_UNSIGNED_INT: return { 16, 16, 16 };
        case SGGColorInternalFormat::SGG_RGB32_INT: return { 32, 32, 32 };
        case SGGColorInternalFormat::SGG_RGB32_UNSIGNED_INT: return { 32, 32, 32 };
        case SGGColorInternalFormat::SGG_RGBA8_INT: return { 8, 8, 8, 8 };
        case SGGColorInternalFormat::SGG_RGBA8_UNSIGNED_INT: return { 8, 8, 8, 8 };
        case SGGColorInternalFormat::SGG_RGBA16_INT: return { 16, 16, 16, 16 };
        case SGGColorInternalFormat::SGG_RGBA16_UNSIGNED_INT: return { 16, 16, 16, 16 };
        case SGGColorInternalFormat::SGG_RGBA32_INT: return { 32, 32, 32, 32 };
        case SGGColorInternalFormat::SGG_RGBA32_UNSIGNED_INT: return { 32, 32, 32, 32 };
        case SGGColorInternalFormat::SGG_COMPRESSED_R: return { };
        case SGGColorInternalFormat::SGG_COMPRESSED_RG: return { };
        case SGGColorInternalFormat::SGG_COMPRESSED_RGB: return { };
        case SGGColorInternalFormat::SGG_COMPRESSED_RGBA: return { };
        case SGGColorInternalFormat::SGG_COMPRESSED_SRGB: return { };
        case SGGColorInternalFormat::SGG_COMPRESSED_SRGBA: return { };
        case SGGColorInternalFormat::SGG_STENCIL_INDEX8: return { 8 };
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT16: return { 16 };
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT24: return { 24 };
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT32: return { 32 };
        case SGGColorInternalFormat::SGG_DEPTH_COMPONENT32F: return { 32 };
        case SGGColorInternalFormat::SGG_DEPTH24_STENCIL8: return { 24, 8 };
        case SGGColorInternalFormat::SGG_DEPTH32F_STENCIL8: return { 32, 8 };
    }

    return { };
}

#endif //SUNGEARENGINE_GRAPHICSDATATYPES_H
