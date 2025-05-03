//
// Created by stuka on 29.08.2023.
//

#include "GL4CubemapTexture.h"
#include "SGCore/Graphics/API/GL/GLGraphicsTypesCaster.h"

// todo: impl 
void SGCore::GL4CubemapTexture::create()
{
    m_isLoaded = true;

    glGenTextures(1, &m_cubemapHandler);

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapHandler);

    std::uint8_t currentPartId = 0;
    for(const auto& part : m_parts)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + currentPartId,
                     0,
                     GLGraphicsTypesCaster::sggInternalFormatToGL(part->m_internalFormat),
                     part->getWidth(),
                     part->getHeight(),
                     0,
                     GLGraphicsTypesCaster::sggFormatToGL(part->m_format),
                     GL_UNSIGNED_BYTE,
                     part->getData().get()
        );

        currentPartId++;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void SGCore::GL4CubemapTexture::subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset)
{
    ITexture2D::subTextureBufferDataOnGAPISide(bytesCount, bytesOffset);
}

void SGCore::GL4CubemapTexture::subTextureDataOnGAPISide(std::size_t areaWidth, std::size_t areaHeight, std::size_t areaOffsetX, std::size_t areaOffsetY, int dataTypeSize)
{
    SG_ASSERT(false,
              fmt::format("OPENGL: Can not do subTextureDataOnGAPISide(...) for cubemap texture: not supported now.\n"
                  "Texture path: '{}'\n"
                  "Texture alias: '{}'",
                  Utils::toUTF8(getPath().resolved().u16string()),
                  getAlias()).c_str());
}

void SGCore::GL4CubemapTexture::createAsFrameBufferAttachment
(const SGCore::Ref<SGCore::IFrameBuffer>& parentFrameBuffer, SGFrameBufferAttachmentType attachmentType)
{

}

void SGCore::GL4CubemapTexture::destroy()
{
    glDeleteTextures(1, &m_cubemapHandler);
}

void SGCore::GL4CubemapTexture::bind(const uint8_t& textureUnit) const
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapHandler);
}

void* SGCore::GL4CubemapTexture::getTextureNativeHandler() const noexcept
{
    return (void*) (intptr_t) m_cubemapHandler;
}

void* SGCore::GL4CubemapTexture::getTextureBufferNativeHandler() const noexcept
{
    return nullptr;
}

glm::vec3 SGCore::GL4CubemapTexture::pickColor(const glm::vec2& inPosition) const noexcept
{
    return glm::vec3();
}

SGCore::GL4CubemapTexture&
SGCore::GL4CubemapTexture::operator=(const Ref<ITexture2D>& other)
{
    return *this;
}
