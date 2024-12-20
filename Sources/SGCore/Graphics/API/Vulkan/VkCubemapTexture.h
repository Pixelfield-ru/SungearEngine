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
        
        void subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) override;
        void subTextureDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) override;
        
        void destroy() override;

        void bind(const std::uint8_t& textureUnit) const override;
        
        void* getTextureNativeHandler() const noexcept override;
        void* getTextureBufferNativeHandler() const noexcept override;

        [[nodiscard]] glm::vec3 pickColor(const glm::vec2& inPosition) const noexcept final;
        
        VkCubemapTexture& operator=(const Ref<ITexture2D>& other) override;
    };
}

#endif //SUNGEARENGINE_VKCUBEMAPTEXTURE_H
