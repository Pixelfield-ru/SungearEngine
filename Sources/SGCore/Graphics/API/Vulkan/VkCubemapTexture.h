//
// Created by stuka on 29.08.2023.
//

#ifndef SUNGEARENGINE_VKCUBEMAPTEXTURE_H
#define SUNGEARENGINE_VKCUBEMAPTEXTURE_H

#include "SGCore/Graphics/API/ICubemapTexture.h"

namespace SGCore
{
    // todo: impl
    class VkCubemapTexture : public ICubemapTexture
    {
    public:
        void create() override;
        void createAsFrameBufferAttachment(const Ref<IFrameBuffer>& parentFrameBuffer, SGFrameBufferAttachmentType attachmentType) override;
        void destroy() override;

        void bind(const std::uint8_t& textureUnit) override;

        VkCubemapTexture& operator=(const Ref<ITexture2D>& other) override;
    };
}

#endif //SUNGEARENGINE_VKCUBEMAPTEXTURE_H
