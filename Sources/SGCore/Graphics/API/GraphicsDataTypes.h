//
// Created by stuka on 22.05.2023.
//

#ifndef SUNGEARENGINE_GRAPHICSDATATYPES_H
#define SUNGEARENGINE_GRAPHICSDATATYPES_H

#include <iostream>
#include <cstdint>

enum SGShaderDefineType
{
    SGG_MATERIAL_TEXTURES_BLOCK_DEFINE,
    SGG_OTHER_DEFINE
};

enum SGDrawMode
{
    SGG_TRIANGLES,
    SGG_LINES,
    SGG_QUADS
};

enum SGFaceType
{
    SGG_FRONT_FACE,
    SGG_BACK_FACE,
    SGG_FRONT_BACK_FACE
};

enum SGPolygonsOrder
{
    // clockwise
    SGG_CW,
    // counter-clockwise
    SGG_CCW
};

enum SGFrameBufferAttachmentType
{
    SGG_DEPTH_ATTACHMENT,
    SGG_DEPTH_STENCIL_ATTACHMENT,

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

    SGG_RENDER_ATTACHMENT
};

enum SGGDataType
{
    SGG_NONE,

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

    // struct marking
    SGG_STRUCT_BEGIN,
    SGG_STRUCT_END
};

enum SGTextureType
{
    SGTP_EMISSIVE,
    SGTP_AMBIENT_OCCLUSION,
    SGTP_AMBIENT,
    SGTP_DIFFUSE_ROUGHNESS,
    SGTP_DIFFUSE,
    SGTP_DISPLACEMENT,
    SGTP_HEIGHT,
    SGTP_NORMALS,
    SGTP_BASE_COLOR,
    SGTP_CLEARCOAT,
    SGTP_EMISSION_COLOR,
    SGTP_LIGHTMAP,
    SGTP_METALNESS,
    SGTP_NORMAL_CAMERA,
    SGTP_OPACITY,
    SGTP_REFLECTION,
    SGTP_SHEEN,
    SGTP_SHININESS,
    SGTP_SPECULAR,
    SGTP_TRANSMISSION,
    SGTP_SHADOW_MAP,
    SGTP_SKYBOX,

    // frame buffers attachments textures types ||| THE TYPES OF ATTACHMENTS HERE SHOULD GO IN A ROW!!!!!
    SGTP_FRAMEBUFFER_DEPTH_ATTACHMENT,
    SGTP_FRAMEBUFFER_DEPTH_STENCIL_ATTACHMENT,
    SGTP_FRAMEBUFFER_COLOR_ATTACHMENT,
    SGTP_FRAMEBUFFER_RENDER_ATTACHMENT
};

static std::string sgTextureTypeToString(const SGTextureType& sgMaterialTextureType)
{
    switch(sgMaterialTextureType)
    {
        case SGTP_EMISSIVE: return "sgmat_emissive";
        case SGTP_AMBIENT_OCCLUSION: return "sgmat_ambientOcclusion";
        case SGTP_AMBIENT: return "sgmat_ambient";
        case SGTP_DIFFUSE_ROUGHNESS: return "sgmat_diffuseRoughness";
        case SGTP_DIFFUSE: return "sgmat_diffuse";
        case SGTP_DISPLACEMENT: return "sgmat_displacement";
        case SGTP_HEIGHT: return "sgmat_height";
        case SGTP_NORMALS: return "sgmat_normals";
        case SGTP_BASE_COLOR: return "sgmat_baseColor";
        case SGTP_CLEARCOAT: return "sgmat_clearcoat";
        case SGTP_EMISSION_COLOR: return "sgmat_emissionColor";
        case SGTP_LIGHTMAP: return "sgmat_lightmap";
        case SGTP_METALNESS: return "sgmat_metalness";
        case SGTP_NORMAL_CAMERA: return "sgmat_normalCamera";
        case SGTP_OPACITY: return "sgmat_opacity";
        case SGTP_REFLECTION: return "sgmat_relfection";
        case SGTP_SHEEN: return "sgmat_sheen";
        case SGTP_SHININESS: return "sgmat_shininess";
        case SGTP_SPECULAR: return "sgmat_specular";
        case SGTP_TRANSMISSION: return "sgmat_transmission";
        case SGTP_SHADOW_MAP: return "sgmat_shadowMap";
        case SGTP_SKYBOX: return "sgmat_skybox";

        case SGTP_FRAMEBUFFER_DEPTH_ATTACHMENT: return "sgmat_frameBufferDepthAttachment";
        case SGTP_FRAMEBUFFER_DEPTH_STENCIL_ATTACHMENT: return "sgmat_frameBufferDepthStencilAttachment";
        case SGTP_FRAMEBUFFER_COLOR_ATTACHMENT: return "sgmat_frameBufferColorAttachment";
        case SGTP_FRAMEBUFFER_RENDER_ATTACHMENT: return "sgmat_frameBufferRenderAttachment";
    }

    return "";
}

static std::uint16_t getSGGDataTypeSizeInBytes(const SGGDataType& dataType) noexcept
{
    int size;

    switch(dataType)
    {
        case SGG_NONE: size = 0; break;

        case SGG_INT: size = 4; break;
        case SGG_INT2: size = 4 * 2; break;
        case SGG_INT3: size = 4 * 3; break;
        case SGG_INT4: size = 4 * 4; break;

        case SGG_FLOAT: size = 4; break;
        case SGG_FLOAT2: size = 4 * 2; break;
        case SGG_FLOAT3: size = 4 * 3; break;
        case SGG_FLOAT4: size = 4 * 4; break;

        case SGG_MAT2: size = 4 * 2 * 2; break;
        case SGG_MAT3: size = 4 * 3 * 3; break;
        case SGG_MAT4: size = 4 * 4 * 4; break;

        case SGG_BOOL: size = 4; break;

        case SGG_STRUCT_BEGIN: size = 0; break;
        case SGG_STRUCT_END: size = 0; break;

        default: size = 0; break;
    }

    return size;
}

static std::uint16_t getSGGDataTypeAlignedSizeInBytes(const SGGDataType& dataType) noexcept
{
    int size;

    switch(dataType)
    {
        case SGG_NONE: size = 0; break;

        case SGG_INT: size = 4; break;
        case SGG_INT2: size = 4 * 2; break;
        // 16
        case SGG_INT3: size = 4 * 4; break;
        case SGG_INT4: size = 4 * 4; break;

        case SGG_FLOAT: size = 4; break;
        case SGG_FLOAT2: size = 4 * 2; break;
        case SGG_FLOAT3: size = 4 * 4; break;
        case SGG_FLOAT4: size = 4 * 4; break;

        // 32
        case SGG_MAT2: size = 4 * 4 * 2; break;
        // 64
        case SGG_MAT3: size = 4 * 4 * 4; break;
        // 64
        case SGG_MAT4: size = 4 * 4 * 4; break;

        case SGG_BOOL: size = 4; break;

        case SGG_STRUCT_BEGIN: size = 0; break;
        case SGG_STRUCT_END: size = 0; break;

        default: size = 0; break;
    }

    return size;
}

enum SGGUsage
{
    SGG_DYNAMIC,
    SGG_STATIC
};

enum SGGColorInternalFormat
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

enum SGGColorFormat
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

    SGG_DEPTH_STENCIL
};

#endif //SUNGEARENGINE_GRAPHICSDATATYPES_H
