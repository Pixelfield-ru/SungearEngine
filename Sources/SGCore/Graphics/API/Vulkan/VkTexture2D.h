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
        
        void subTextureDataOnGAPISide(std::size_t areaWidth, std::size_t areaHeight, std::size_t areaOffsetX, std::size_t areaOffsetY, int dataTypeSize) noexcept override;
        
        void destroy() noexcept final;

        void bind(const std::uint8_t& textureUnit) const noexcept final;
        
        void* getTextureNativeHandler() const noexcept override;
        void* getTextureBufferNativeHandler() const noexcept override;

        VkTexture2D& operator=(const Ref<ITexture2D>& other) final;
    };
}

#endif //SUNGEARENGINE_VKTEXTURE2D_H
