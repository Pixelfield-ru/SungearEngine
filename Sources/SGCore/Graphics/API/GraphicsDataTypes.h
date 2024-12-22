//
// Created by stuka on 22.05.2023.
//

#ifndef SUNGEARENGINE_GRAPHICSDATATYPES_H
#define SUNGEARENGINE_GRAPHICSDATATYPES_H

#include <SGCore/pch.h>

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

enum class SGStencilFunc
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
    SGG_POINTS
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

enum class SGTextureType
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

    SGTT_SHADOW_MAP2D,

    SGTT_NONE
};

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

    return SGGDataType::SGG_NONE;
}

static SGTextureType sgStandardTextureFromString(const std::string& textureType) noexcept
{
    if(textureType == "SGTT_EMISSIVE")
    {
        return SGTextureType::SGTT_EMISSIVE;
    }
    else if(textureType == "SGTT_AMBIENT_OCCLUSION")
    {
        return SGTextureType::SGTT_AMBIENT_OCCLUSION;
    }
    else if(textureType == "SGTT_AMBIENT")
    {
        return SGTextureType::SGTT_AMBIENT;
    }
    else if(textureType == "SGTT_DIFFUSE_ROUGHNESS")
    {
        return SGTextureType::SGTT_DIFFUSE_ROUGHNESS;
    }
    else if(textureType == "SGTT_DIFFUSE")
    {
        return SGTextureType::SGTT_DIFFUSE;
    }
    else if(textureType == "SGTT_DISPLACEMENT")
    {
        return SGTextureType::SGTT_DISPLACEMENT;
    }
    else if(textureType == "SGTT_HEIGHT")
    {
        return SGTextureType::SGTT_HEIGHT;
    }
    else if(textureType == "SGTT_NORMALS")
    {
        return SGTextureType::SGTT_NORMALS;
    }
    else if(textureType == "SGTT_BASE_COLOR")
    {
        return SGTextureType::SGTT_BASE_COLOR;
    }
    else if(textureType == "SGTT_CLEARCOAT")
    {
        return SGTextureType::SGTT_CLEARCOAT;
    }
    else if(textureType == "SGTT_EMISSION_COLOR")
    {
        return SGTextureType::SGTT_EMISSION_COLOR;
    }
    else if(textureType == "SGTT_LIGHTMAP")
    {
        return SGTextureType::SGTT_LIGHTMAP;
    }
    else if(textureType == "SGTT_METALNESS")
    {
        return SGTextureType::SGTT_METALNESS;
    }
    else if(textureType == "SGTT_NORMAL_CAMERA")
    {
        return SGTextureType::SGTT_NORMAL_CAMERA;
    }
    else if(textureType == "SGTT_OPACITY")
    {
        return SGTextureType::SGTT_OPACITY;
    }
    else if(textureType == "SGTT_REFLECTION")
    {
        return SGTextureType::SGTT_REFLECTION;
    }
    else if(textureType == "SGTT_SHEEN")
    {
        return SGTextureType::SGTT_SHEEN;
    }
    else if(textureType == "SGTT_SHININESS")
    {
        return SGTextureType::SGTT_SHININESS;
    }
    else if(textureType == "SGTT_SPECULAR")
    {
        return SGTextureType::SGTT_SPECULAR;
    }
    else if(textureType == "SGTT_TRANSMISSION")
    {
        return SGTextureType::SGTT_TRANSMISSION;
    }
    else if(textureType == "SGTT_SKYBOX")
    {
        return SGTextureType::SGTT_SKYBOX;
    }
    else if(textureType == "SGTT_SHADOW_MAP2D")
    {
        return SGTextureType::SGTT_SHADOW_MAP2D;
    }
    else if(textureType == "SGTT_NONE")
    {
        return SGTextureType::SGTT_NONE;
    }

    return SGTextureType::SGTT_NONE;
}

static std::string sgStandardTextureTypeToString(const SGTextureType& sgMaterialTextureType) noexcept
{
    switch(sgMaterialTextureType)
    {
        case SGTextureType::SGTT_EMISSIVE: return "SGTT_EMISSIVE";
        case SGTextureType::SGTT_AMBIENT_OCCLUSION: return "SGTT_AMBIENT_OCCLUSION";
        case SGTextureType::SGTT_AMBIENT: return "SGTT_AMBIENT";
        case SGTextureType::SGTT_DIFFUSE_ROUGHNESS: return "SGTT_DIFFUSE_ROUGHNESS";
        case SGTextureType::SGTT_DIFFUSE: return "SGTT_DIFFUSE";
        case SGTextureType::SGTT_DISPLACEMENT: return "SGTT_DISPLACEMENT";
        case SGTextureType::SGTT_HEIGHT: return "SGTT_HEIGHT";
        case SGTextureType::SGTT_NORMALS: return "SGTT_NORMALS";
        case SGTextureType::SGTT_BASE_COLOR: return "SGTT_BASE_COLOR";
        case SGTextureType::SGTT_CLEARCOAT: return "SGTT_CLEARCOAT";
        case SGTextureType::SGTT_EMISSION_COLOR: return "SGTT_EMISSION_COLOR";
        case SGTextureType::SGTT_LIGHTMAP: return "SGTT_LIGHTMAP";
        case SGTextureType::SGTT_METALNESS: return "SGTT_METALNESS";
        case SGTextureType::SGTT_NORMAL_CAMERA: return "SGTT_NORMAL_CAMERA";
        case SGTextureType::SGTT_OPACITY: return "SGTT_OPACITY";
        case SGTextureType::SGTT_REFLECTION: return "SGTT_REFLECTION";
        case SGTextureType::SGTT_SHEEN: return "SGTT_SHEEN";
        case SGTextureType::SGTT_SHININESS: return "SGTT_SHININESS";
        case SGTextureType::SGTT_SPECULAR: return "SGTT_SPECULAR";
        case SGTextureType::SGTT_TRANSMISSION: return "SGTT_TRANSMISSION";
        case SGTextureType::SGTT_SKYBOX: return "SGTT_SKYBOX";

        case SGTextureType::SGTT_SHADOW_MAP2D: return "SGTT_SHADOW_MAP2D";

        case SGTextureType::SGTT_NONE: return "SGTT_NONE";
    }

    return "";
}

static const char* sgStandardTextureTypeNameToStandardUniformName(const SGTextureType& sgMaterialTextureType) noexcept
{
    switch(sgMaterialTextureType)
    {
        case SGTextureType::SGTT_EMISSIVE: return "mat_emissiveSamplers";
        case SGTextureType::SGTT_AMBIENT_OCCLUSION: return "mat_ambientOcclusionSamplers";
        case SGTextureType::SGTT_AMBIENT: return "mat_ambientSamplers";
        case SGTextureType::SGTT_DIFFUSE_ROUGHNESS: return "mat_diffuseRoughnessSamplers";
        case SGTextureType::SGTT_DIFFUSE: return "mat_diffuseSamplers";
        case SGTextureType::SGTT_DISPLACEMENT: return "mat_displacementSamplers";
        case SGTextureType::SGTT_HEIGHT: return "mat_heightSamplers";
        case SGTextureType::SGTT_NORMALS: return "mat_normalsSamplers";
        case SGTextureType::SGTT_BASE_COLOR: return "mat_baseColorSamplers";
        case SGTextureType::SGTT_CLEARCOAT: return "mat_clearCoatSamplers";
        case SGTextureType::SGTT_EMISSION_COLOR: return "mat_emissionColorSamplers";
        case SGTextureType::SGTT_LIGHTMAP: return "mat_lightmapSamplers";
        case SGTextureType::SGTT_METALNESS: return "mat_metalnessSamplers";
        case SGTextureType::SGTT_NORMAL_CAMERA: return "mat_normalCameraSamplers";
        case SGTextureType::SGTT_OPACITY: return "mat_opacitySamplers";
        case SGTextureType::SGTT_REFLECTION: return "mat_reflectionSamplers";
        case SGTextureType::SGTT_SHEEN: return "mat_sheenSamplers";
        case SGTextureType::SGTT_SHININESS: return "mat_shininessSamplers";
        case SGTextureType::SGTT_SPECULAR: return "mat_specularSamplers";
        case SGTextureType::SGTT_TRANSMISSION: return "mat_transmissionSamplers";
        case SGTextureType::SGTT_SKYBOX: return "mat_skyboxSamplers";
        
        case SGTextureType::SGTT_SHADOW_MAP2D: return "mat_shadowMap2DSamplers";
        
        case SGTextureType::SGTT_NONE: return "noneSamplers";
    }
    
    return "noneSamplers";
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
    SGG_RED,
    
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

#endif //SUNGEARENGINE_GRAPHICSDATATYPES_H
