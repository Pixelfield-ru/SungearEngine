//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKTEXTURE2D_H
#define SUNGEARENGINE_VKTEXTURE2D_H

#include "SGCore/Graphics/API/ITexture2D.h"
#include "RHI/VulkanTexture.h"
#include "RHI/VulkanGPUBuffer.h"

namespace SGCore
{
    /// Legacy ITexture2D facade over a VulkanTexture. Unit-model bind() is a no-op: on Vulkan the
    /// texture reaches shaders through descriptor sets (see docs/RHI_DESIGN.md, textures).
    class VkTexture2D : public ITexture2D
    {
    public:
        ~VkTexture2D() noexcept override;

        void create() final;
        void createAsFrameBufferAttachment(IFrameBuffer* parentFrameBuffer, SGFrameBufferAttachmentType attachmentType) override;

        void destroyOnGPU() noexcept final;

        void bind(const std::uint8_t& textureUnit) const noexcept final;

        void* getTextureNativeHandler() const noexcept override;
        void* getTextureBufferNativeHandler() const noexcept override;

        VkTexture2D& operator=(const Ref<ITexture2D>& other) final;

        [[nodiscard]] const Ref<VulkanTexture>& getVulkanTexture() const noexcept { return m_vulkanTexture; }

    private:
        void subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) noexcept override;

        /// SG_TEXTURE_BUFFER path: a texel buffer, not an image. Kept apart from m_vulkanTexture
        /// because the two are different descriptor kinds — a samplerBuffer reads a VkBufferView.
        void createAsTexelBuffer() noexcept;

        void subTextureDataOnGAPISide(const std::uint8_t* data, std::size_t areaWidth, std::size_t areaHeight, std::size_t areaOffsetX, std::size_t areaOffsetY) noexcept override;

        /// Copies pixels (in the texture's declared format) into the image region and leaves it in
        /// SHADER_READ_ONLY. Expands RGB data to RGBA when the Vulkan format was widened.
        void uploadRegion(const std::uint8_t* data, std::uint32_t width, std::uint32_t height, std::uint32_t x, std::uint32_t y) noexcept;

        Ref<VulkanTexture> m_vulkanTexture;
        /// Set instead of m_vulkanTexture when m_type == SG_TEXTURE_BUFFER.
        Ref<VulkanGPUBuffer> m_texelBuffer;
    };
}

#endif //SUNGEARENGINE_VKTEXTURE2D_H
