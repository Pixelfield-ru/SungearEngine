//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_VKCUBEMAPTEXTURE_H
#define SUNGEARENGINE_VKCUBEMAPTEXTURE_H

#include "SGCore/Graphics/API/ICubemapTexture.h"
#include "RHI/VulkanTexture.h"

namespace SGCore
{
    /// Legacy ICubemapTexture facade: the six m_parts are uploaded into the layers of one cube image.
    /// Like VkTexture2D, bind() only records the texture unit — the descriptor set is assembled by
    /// VkShader at draw time.
    class VkCubemapTexture : public ICubemapTexture
    {
    public:
        ~VkCubemapTexture() noexcept override;

        void create() override;
        void createAsFrameBufferAttachment(IFrameBuffer* parentFrameBuffer, SGFrameBufferAttachmentType attachmentType) override;
        void subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) override;
        void subTextureDataOnGAPISide(const std::uint8_t* data, std::size_t areaWidth, std::size_t areaHeight, std::size_t areaOffsetX, std::size_t areaOffsetY) override;
        void destroyOnGPU() override;
        void bind(const std::uint8_t& textureUnit) const override;
        void* getTextureNativeHandler() const noexcept override;
        void* getTextureBufferNativeHandler() const noexcept override;
        VkCubemapTexture& operator=(const Ref<ITexture2D>& other) override;

        [[nodiscard]] const Ref<VulkanTexture>& getVulkanTexture() const noexcept { return m_vulkanTexture; }

    private:
        Ref<VulkanTexture> m_vulkanTexture;
    };
}

#endif //SUNGEARENGINE_VKCUBEMAPTEXTURE_H
