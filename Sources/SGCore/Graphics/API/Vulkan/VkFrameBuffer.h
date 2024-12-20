//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_VKFRAMEBUFFER_H
#define SUNGEARENGINE_VKFRAMEBUFFER_H

#include "SGCore/Graphics/API/IFrameBuffer.h"

namespace SGCore
{
    class VkFrameBuffer : public IFrameBuffer
    {
    public:
        void bind() const override;
        void unbind() const override;

        void create() override;
        void destroy() override;
        
        void addAttachment(SGFrameBufferAttachmentType,
                           SGGColorFormat format, SGGColorInternalFormat internalFormat,
                           const int& mipLevel,
                           const int& layer) override;

        [[nodiscard]] glm::vec3 readPixelsFromAttachment(const glm::vec2& mousePos, SGFrameBufferAttachmentType attachmentType) const noexcept final;
    };
}

#endif //SUNGEARENGINE_VKFRAMEBUFFER_H
