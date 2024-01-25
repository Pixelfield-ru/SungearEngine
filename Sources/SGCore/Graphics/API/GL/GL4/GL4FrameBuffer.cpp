//
// Created by stuka on 23.07.2023.
//

#include "GL4FrameBuffer.h"

#include "SGCore/Graphics/API/GL/GLGraphicsTypesCaster.h"
#include "SGCore/Main/CoreMain.h"
#include "GL4Texture2D.h"

#include "SGCore/Graphics/API/GL/DeviceGLInfo.h"
#include "spdlog/spdlog.h"

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::bindAttachment
(const SGFrameBufferAttachmentType& attachmentType, const std::uint8_t& textureBlock)
{
    auto foundAttachment = m_attachments.find(attachmentType);

    if(foundAttachment != m_attachments.end())
    {
        const auto& attachment = foundAttachment->second;

        attachment->bind(textureBlock);
    }

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::bindAttachmentToRead
(const SGFrameBufferAttachmentType& attachmentType)
{
    if(attachmentType >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
       attachmentType <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + (attachmentType - SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0));
    }

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::bindAttachmentToDraw
(const SGFrameBufferAttachmentType& attachmentType)
{
    if(attachmentType >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
       attachmentType <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
    {
        glDrawBuffer(GL_COLOR_ATTACHMENT0 + (attachmentType - SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0));
    }

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::bindAttachmentsToRead
(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes)
{
    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::bindAttachmentsToDraw
(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes)
{
    GLenum attachmentsToBind[attachmentsTypes.size()];

    std::uint8_t curAttachment = 0;
    for(const auto& type: attachmentsTypes)
    {
        if(type >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
           type <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
        {
            attachmentsToBind[curAttachment] = GL_COLOR_ATTACHMENT0 + (type - SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0);
        }

        ++curAttachment;
    }

    glDrawBuffers(attachmentsTypes.size(), attachmentsToBind);

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::bindAttachmentsToDraw
(const std::set<SGFrameBufferAttachmentType>& attachmentsTypes)
{
    GLenum attachmentsToBind[attachmentsTypes.size()];

    std::uint8_t curAttachment = 0;
    for(const auto& type: attachmentsTypes)
    {
        if(type >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
           type <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
        {
            attachmentsToBind[curAttachment] = GL_COLOR_ATTACHMENT0 + (type - SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0);
        }

        ++curAttachment;
    }

    glDrawBuffers(attachmentsTypes.size(), attachmentsToBind);

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::unbindAttachmentToRead()
{
    glReadBuffer(GL_NONE);

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::unbindAttachmentToDraw()
{
    glDrawBuffer(GL_NONE);

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::bind()
{
    glViewport(0, 0, m_width, m_height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_handler);

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    int wndWidth;
    int wndHeight;
    CoreMain::getWindow().getSize(wndWidth, wndHeight);
    glViewport(0, 0, wndWidth, wndHeight);

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::create()
{
    glGenFramebuffers(1, &m_handler);

    return shared_from_this();
}

void SGCore::GL4FrameBuffer::destroy()
{
    glDeleteFramebuffers(1, &m_handler);
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::clear()
{
    glClearColor(m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::addAttachment(SGFrameBufferAttachmentType attachmentType,
                                                                            SGGColorFormat format,
                                                                            SGGColorInternalFormat internalFormat,
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

        if(colorAttachments >= DeviceGLInfo::getMaxFBColorAttachments())
        {
            spdlog::error(
                    "It is not possible to add more color attachments for framebuffer. Current color attachments count: {0}. Max color attachments count: {1}.\n{2}",
                    colorAttachments,
                    DeviceGLInfo::getMaxFBColorAttachments(),
                    SGUtils::Utils::sourceLocationToString(std::source_location::current()));

            return shared_from_this();
        }

        if(curAttachmentType == attachmentType)
        {
            spdlog::error("Error when adding an attachment to the framebuffer: "
                       "an attachment with this type already exists.\n{0}",
                       SGUtils::Utils::sourceLocationToString(std::source_location::current()));

            return shared_from_this();
        }
    }

    auto& newAttachment = m_attachments[attachmentType];
    newAttachment = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());

    newAttachment->m_width = m_width;
    newAttachment->m_height = m_height;
    newAttachment->m_format = format;
    newAttachment->m_internalFormat = internalFormat;
    newAttachment->m_mipLevel = mipLevel;
    newAttachment->m_layer = layer;
    newAttachment->m_useMultisampling = useMultisampling;
    newAttachment->m_multisamplingSamplesCount = multisamplingSamplesCount;

    newAttachment->createAsFrameBufferAttachment(shared_from_this(), attachmentType);

    // todo: make check for errors function
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        switch (status)
        {
            case GL_FRAMEBUFFER_UNDEFINED:
                // Обработка ошибки: Фреймбуфер не определен
                spdlog::error("Error when adding attachment to framebuffer: GL_FRAMEBUFFER_UNDEFINED.\n{0}",
                              SG_CURRENT_LOCATION_STR);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                // Обработка ошибки: Неполное прикрепление
                spdlog::error("Error when adding attachment to framebuffer: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT.\n{0}",
                              SG_CURRENT_LOCATION_STR);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                // Обработка ошибки: Отсутствует прикрепление
                spdlog::error("Error when adding attachment to framebuffer: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT.\n{0}",
                              SG_CURRENT_LOCATION_STR);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                // Обработка ошибки: Неправильный буфер рисования
                spdlog::error("Error when adding attachment to framebuffer: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER.\n{0}",
                              SG_CURRENT_LOCATION_STR);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                // Обработка ошибки: Неправильный буфер чтения
                spdlog::error("Error when adding attachment to framebuffer: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER.\n{0}",
                              SG_CURRENT_LOCATION_STR);
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                // Обработка ошибки: Неподдерживаемый формат фреймбуфера
                spdlog::error("Error when adding attachment to framebuffer: GL_FRAMEBUFFER_UNSUPPORTED.\n{0}",
                              SG_CURRENT_LOCATION_STR);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                // Обработка ошибки: Неправильное мультисэмплирование
                spdlog::error("Error when adding attachment to framebuffer: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE.\n{0}",
                              SG_CURRENT_LOCATION_STR);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                // Обработка ошибки: Неправильные цели слоя
                spdlog::error("Error when adding attachment to framebuffer: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS.\n{0}",
                              SG_CURRENT_LOCATION_STR);
                break;
            default:
                // Обработка ошибки: Неизвестная ошибка
                spdlog::error("Error when adding attachment to framebuffer: unknown error.", SG_CURRENT_LOCATION_STR);
                break;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer>
SGCore::GL4FrameBuffer::addAttachment(SGFrameBufferAttachmentType attachmentType,
                                      SGGColorFormat format,
                                      SGGColorInternalFormat internalFormat,
                                      const int& mipLevel,
                                      const int& layer)
{
    return addAttachment(attachmentType, format, internalFormat, mipLevel, layer, false, 8);
}
