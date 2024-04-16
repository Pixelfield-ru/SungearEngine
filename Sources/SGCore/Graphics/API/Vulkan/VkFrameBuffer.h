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
        void bind() override;
        void unbind() override;

        void create() override;
        void destroy() override;
        
        void addAttachment(SGFrameBufferAttachmentType,
                           SGGColorFormat format, SGGColorInternalFormat internalFormat,
                           const int& mipLevel,
                           const int& layer) override;
    };
}

#endif //SUNGEARENGINE_VKFRAMEBUFFER_H
