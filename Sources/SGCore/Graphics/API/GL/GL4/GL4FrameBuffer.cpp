//
// Created by stuka on 23.07.2023.
//

#include "GL4FrameBuffer.h"

#include "SGCore/Graphics/API/GL/GLGraphicsTypesCaster.h"
#include "SGCore/Main/CoreMain.h"
#include "GL4Texture2D.h"

#include "SGCore/Graphics/API/GL/DeviceGLInfo.h"

void SGCore::GL4FrameBuffer::bindAttachment
(const SGFrameBufferAttachmentType& attachmentType, const std::uint8_t& textureBlock)
{
    auto foundAttachment = m_attachments.find(attachmentType);

    if(foundAttachment != m_attachments.end())
    {
        const auto& attachment = foundAttachment->second;

        attachment->bind(textureBlock);
    }
}

void SGCore::GL4FrameBuffer::bindAttachmentToReadFrom
(const SGFrameBufferAttachmentType& attachmentType)
{
    if(attachmentType >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
       attachmentType <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + (std::to_underlying(attachmentType) -
                std::to_underlying(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0)));
    }
}

void SGCore::GL4FrameBuffer::bindAttachmentToDrawIn
(const SGFrameBufferAttachmentType& attachmentType)
{
    if(attachmentType >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
       attachmentType <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
    {
        const GLenum buffers[] = {
            GLenum(GL_COLOR_ATTACHMENT0 + (std::to_underlying(attachmentType) - std::to_underlying(
                                               SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0)))
        };
        glDrawBuffers(1, buffers);
    }
}

void SGCore::GL4FrameBuffer::bindAttachmentsToReadFrom
(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes)
{

}

void SGCore::GL4FrameBuffer::bindAttachmentsToDrawIn
(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes)
{
    std::vector<GLenum> attachmentsToBind;
    attachmentsToBind.resize(attachmentsTypes.size());

    std::uint8_t curAttachment = 0;
    for(const auto& type: attachmentsTypes)
    {
        if(type >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
           type <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
        {
            attachmentsToBind[curAttachment] = GL_COLOR_ATTACHMENT0 + (std::to_underlying(type) -
                    std::to_underlying(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0));
        }

        ++curAttachment;
    }

    glDrawBuffers(attachmentsTypes.size(), attachmentsToBind.data());
}

void SGCore::GL4FrameBuffer::bindAttachmentsToDrawIn
(const std::set<SGFrameBufferAttachmentType>& attachmentsTypes)
{
    std::vector<GLenum> attachmentsToBind;
    attachmentsToBind.resize(attachmentsTypes.size());

    std::uint8_t curAttachment = 0;
    for(const auto& type: attachmentsTypes)
    {
        if(type >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
           type <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
        {
            attachmentsToBind[curAttachment] = GL_COLOR_ATTACHMENT0 + (std::to_underlying(type) -
                    std::to_underlying(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0));
        }

        ++curAttachment;
    }

    glDrawBuffers(attachmentsTypes.size(), attachmentsToBind.data());
}

void SGCore::GL4FrameBuffer::unbindAttachmentToReadFrom()
{
    glReadBuffer(GL_NONE);
}

void SGCore::GL4FrameBuffer::unbindAttachmentToDrawIn()
{
    const GLenum buf[] = { GL_NONE };
    glDrawBuffers(1, buf);
}

void SGCore::GL4FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_handler);
    // useStates();

    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(m_viewportPosX, m_viewportPosY, m_viewportWidth, m_viewportHeight);
}

void SGCore::GL4FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    int wndWidth;
    int wndHeight;
    CoreMain::getWindow().getSize(wndWidth, wndHeight);
    glViewport(0, 0, wndWidth, wndHeight);
}

void SGCore::GL4FrameBuffer::useStates() const noexcept
{
    for(const auto& attachmentIt : m_attachments)
    {
        const auto& attachmentType = attachmentIt.first;
        const auto& attachment = attachmentIt.second;

        if(isColorAttachment(attachmentType))
        {
            attachment->m_blendingState.use();
        }
    }
}

void SGCore::GL4FrameBuffer::create()
{
    glGenFramebuffers(1, &m_handler);
}

void SGCore::GL4FrameBuffer::destroy()
{
    glDeleteFramebuffers(1, &m_handler);
}

void SGCore::GL4FrameBuffer::clear()
{
    // glClearColor(m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    for(const auto& attachmentIt : m_attachments)
    {
        clearAttachment(attachmentIt.first);
    }
}

void SGCore::GL4FrameBuffer::clearAttachment(const SGFrameBufferAttachmentType& attachmentType)
{
    if(attachmentType >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
       attachmentType <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
    {
        const auto glAttachmentType = (std::to_underlying(attachmentType) -
                                     std::to_underlying(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0));

        // glDrawBuffer(GL_COLOR_ATTACHMENT0 + glAttachmentType);
        // glClear(GL_COLOR_BUFFER_BIT);
        // auto attachment = getAttachment(attachmentType);
        // glClearColor(attachment->m_clearColor[0], attachment->m_clearColor[1], attachment->m_clearColor[2], attachment->m_clearColor[3]);
        // glClear(GL_COLOR_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, glAttachmentType, &getAttachment(attachmentType)->m_clearColor[0]);
    }
    else if(attachmentType >= SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0 &&
            attachmentType <= SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT9)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    else if(attachmentType >= SGFrameBufferAttachmentType::SGG_DEPTH_STENCIL_ATTACHMENT0 &&
            attachmentType <= SGFrameBufferAttachmentType::SGG_DEPTH_STENCIL_ATTACHMENT9)
    {
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
}

void SGCore::GL4FrameBuffer::addAttachment(SGFrameBufferAttachmentType attachmentType,
                                           SGGColorFormat format,
                                           SGGColorInternalFormat internalFormat,
                                           SGGDataType dataType,
                                           const int& mipLevel,
                                           const int& layer,
                                           bool useMultisampling,
                                           std::uint8_t multisamplingSamplesCount)
{
    std::uint8_t colorAttachments = 0;

    for(const auto& attachmentPair : m_attachments)
    {
        auto curAttachmentType = attachmentPair.first;

        if(isColorAttachment(curAttachmentType))
        {
            ++colorAttachments;
        }

        if (colorAttachments >= DeviceGLInfo::getMaxFBColorAttachments())
        {
            LOG_E(SGCORE_TAG,
                  "It is not possible to add more color attachments for framebuffer. Current color attachments count: {}. Max color attachments count: {}.\n{}",
                  colorAttachments,
                  DeviceGLInfo::getMaxFBColorAttachments(),
                  SGCore::Utils::sourceLocationToString(std::source_location::current()));
            
            return;
        }

        if(curAttachmentType == attachmentType)
        {
            LOG_E(SGCORE_TAG,
                  "Error when adding an attachment to the framebuffer: "
                  "an attachment with this type already exists.\n{}",
                  SGCore::Utils::sourceLocationToString(std::source_location::current()));

            return;
        }
    }

    auto& newAttachment = m_attachments[attachmentType];
    newAttachment = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());

    newAttachment->resize(m_width, m_height, true);
    newAttachment->m_format = format;
    newAttachment->m_internalFormat = internalFormat;
    newAttachment->m_mipLevel = mipLevel;
    newAttachment->m_layer = layer;
    newAttachment->m_useMultisampling = useMultisampling;
    newAttachment->m_multisamplingSamplesCount = multisamplingSamplesCount;
    newAttachment->m_dataType = dataType;

    newAttachment->createAsFrameBufferAttachment(this, attachmentType);

    // todo: make check for errors function
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        switch (status)
        {
            case GL_FRAMEBUFFER_UNDEFINED:
                // Обработка ошибки: Фреймбуфер не определен
                LOG_E(SGCORE_TAG,
                      "Error when adding attachment to framebuffer: GL_FRAMEBUFFER_UNDEFINED.\n{0}",
                      SG_CURRENT_LOCATION_STR);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                // Обработка ошибки: Неполное прикрепление
                LOG_E(SGCORE_TAG,
                      "Error when adding attachment to framebuffer: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT.\n{0}",
                      SG_CURRENT_LOCATION_STR);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                // Обработка ошибки: Отсутствует прикрепление
                LOG_E(SGCORE_TAG,
                      "Error when adding attachment to framebuffer: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT.\n{0}",
                      SG_CURRENT_LOCATION_STR);
                break;
#ifdef GL_VERSION_1_0
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                // Обработка ошибки: Неправильный буфер рисования
                LOG_E(SGCORE_TAG,
                      "Error when adding attachment to framebuffer: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER.\n{0}",
                      SG_CURRENT_LOCATION_STR);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                // Обработка ошибки: Неправильный буфер чтения
                LOG_E(SGCORE_TAG,
                      "Error when adding attachment to framebuffer: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER.\n{0}",
                      SG_CURRENT_LOCATION_STR);
                break;
#endif
            case GL_FRAMEBUFFER_UNSUPPORTED:
                // Обработка ошибки: Неподдерживаемый формат фреймбуфера
                LOG_E(SGCORE_TAG,
                      "Error when adding attachment to framebuffer: GL_FRAMEBUFFER_UNSUPPORTED.\n{0}",
                      SG_CURRENT_LOCATION_STR);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                // Обработка ошибки: Неправильное мультисэмплирование
                LOG_E(SGCORE_TAG,
                      "Error when adding attachment to framebuffer: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE.\n{0}",
                      SG_CURRENT_LOCATION_STR);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                // Обработка ошибки: Неправильные цели слоя
                LOG_E(SGCORE_TAG,
                      "Error when adding attachment to framebuffer: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS.\n{0}",
                      SG_CURRENT_LOCATION_STR);
                break;
            default:
                // Обработка ошибки: Неизвестная ошибка
                LOG_E(SGCORE_TAG,
                      "Error when adding attachment to framebuffer: unknown error.\n{}",
                      SG_CURRENT_LOCATION_STR);
                break;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void
SGCore::GL4FrameBuffer::addAttachment(SGFrameBufferAttachmentType attachmentType,
                                      SGGColorFormat format,
                                      SGGColorInternalFormat internalFormat,
                                      SGGDataType dataType,
                                      const int& mipLevel,
                                      const int& layer)
{
    addAttachment(attachmentType, format, internalFormat, dataType, mipLevel, layer, false, 8);
}

void SGCore::GL4FrameBuffer::attachAttachment(const SGCore::Ref<SGCore::ITexture2D>& otherAttachment) noexcept
{
    if(isDepthStencilAttachment(otherAttachment->getFrameBufferAttachmentType()))
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, (intptr_t) otherAttachment->getTextureNativeHandler(), 0);
    }
}

glm::vec3 SGCore::GL4FrameBuffer::readPixelsFromAttachment(const glm::vec2& mousePos,
                                                           SGFrameBufferAttachmentType attachmentType) const noexcept
{
    GLfloat pixel[3];

    glBindFramebuffer(GL_FRAMEBUFFER, m_handler);

    if(isColorAttachment(attachmentType))
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + (std::to_underlying(attachmentType) -
                                             std::to_underlying(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0)));
    }

    auto attachment = getAttachment(attachmentType);

    GLenum format = GL_RGB;
    if(isDepthAttachment(attachmentType))
    {
        format = GL_DEPTH_COMPONENT;
    }
    else if(isDepthStencilAttachment(attachmentType))
    {
        format = GL_DEPTH_STENCIL;
    }

    glReadPixels(mousePos.x, mousePos.y, 1, 1,
                 // GLGraphicsTypesCaster::sggFormatToGL(attachment->m_format),
                 format,
                 GL_FLOAT, pixel);

    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return { pixel[0], pixel[1], pixel[2] };
}

