//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKTEXTURE2D_H
#define SUNGEARENGINE_VKTEXTURE2D_H

#include "SGCore/Graphics/API/ITexture2D.h"

namespace SGCore
{
    // TODO: impl VkTexture2D
    class VkTexture2D : public ITexture2D
    {
    public:
        ~VkTexture2D() noexcept;

        void create() final;
        void createAsFrameBufferAttachment(const Ref<IFrameBuffer>& parentFrameBuffer, SGFrameBufferAttachmentType attachmentType) override;
        
        void subTextureDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) noexcept override;
        
        void destroy() noexcept final;

        void bind(const std::uint8_t& textureUnit) const noexcept final;
        
        void* getTextureNativeHandler() const noexcept override;
        void* getTextureBufferNativeHandler() const noexcept override;

        [[nodiscard]] glm::vec3 pickColor(const glm::vec2& inPosition) const noexcept final;

        VkTexture2D& operator=(const Ref<ITexture2D>& other) final;
    };
}

#endif //SUNGEARENGINE_VKTEXTURE2D_H
