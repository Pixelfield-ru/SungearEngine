//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <string>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/IGPUObject.h"
#include "SGCore/Main/CoreGlobals.h"
#include "VulkanContext.h"

namespace SGCore
{
    struct VulkanTextureDesc
    {
        std::uint32_t m_width = 1;
        std::uint32_t m_height = 1;
        VkFormat m_format = VK_FORMAT_R8G8B8A8_UNORM;
        VkImageUsageFlags m_usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        std::uint32_t m_mipLevels = 1;
        std::uint32_t m_arrayLayers = 1;
        VkSampleCountFlagBits m_samples = VK_SAMPLE_COUNT_1_BIT;
        VkFilter m_filter = VK_FILTER_LINEAR;
        VkSamplerAddressMode m_addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        /// Cube map: six array layers behind a VK_IMAGE_VIEW_TYPE_CUBE view (skybox, reflections).
        bool m_cube { };
        std::string m_debugName;
    };

    /// A Vulkan image with its allocation, a full view, a sampler and the CPU-side layout tracker.
    /// The legacy VkTexture2D / VkFrameBuffer facades and the swapchain wrap their images in it;
    /// command lists transition it by recordTransition().
    class SGCORE_EXPORT VulkanTexture final : public IGPUObject
    {
    public:
        VulkanTexture(const VulkanTexture&) = delete;
        VulkanTexture& operator=(const VulkanTexture&) = delete;
        ~VulkanTexture() override;

        /// Creates an owned image (VMA, device-local).
        [[nodiscard]] static Ref<VulkanTexture> create(VulkanContext& context, const VulkanTextureDesc& desc) noexcept;
        /// Wraps an image the swapchain owns: only the view is created and destroyed here.
        [[nodiscard]] static Ref<VulkanTexture> wrap(VulkanContext& context, VkImage image, VkFormat format,
                                                     std::uint32_t width, std::uint32_t height, const std::string& debugName) noexcept;

        /// Records a full-subresource layout transition (synchronization2) and updates the tracker.
        /// No-op when the image already sits in newLayout.
        void recordTransition(VkCommandBuffer commandBuffer, VkImageLayout newLayout) noexcept;

        [[nodiscard]] VkImage getImage() const noexcept { return m_image; }
        [[nodiscard]] VkImageView getView() const noexcept { return m_view; }
        [[nodiscard]] VkSampler getSampler() const noexcept { return m_sampler; }
        [[nodiscard]] VkFormat getFormat() const noexcept { return m_format; }
        [[nodiscard]] VkExtent2D getExtent() const noexcept { return m_extent; }
        [[nodiscard]] VkImageAspectFlags getAspect() const noexcept { return m_aspect; }
        [[nodiscard]] VkImageLayout getLayout() const noexcept { return m_layout; }
        [[nodiscard]] std::uint32_t getMipLevels() const noexcept { return m_mipLevels; }
        [[nodiscard]] bool isDepth() const noexcept { return (m_aspect & VK_IMAGE_ASPECT_DEPTH_BIT) != 0; }

        /// Forces the tracker (e.g. after the presentation engine handed the image back as UNDEFINED).
        void setLayout(VkImageLayout layout) noexcept { m_layout = layout; }

        /// Destroys the Vulkan objects; the texture becomes an empty shell (getImage() == null).
        void releaseGPU() noexcept;

        /// Stage/access pair a layout is used with, for barriers.
        static void layoutToStageAccess(VkImageLayout layout, VkPipelineStageFlags2& stage, VkAccessFlags2& access) noexcept;
        static VkImageAspectFlags aspectOfFormat(VkFormat format) noexcept;

    private:
        VulkanTexture() = default;

        std::shared_ptr<VulkanContext> m_context;
        VkImage m_image = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        VkImageView m_view = VK_NULL_HANDLE;
        VkSampler m_sampler = VK_NULL_HANDLE;
        VkFormat m_format = VK_FORMAT_UNDEFINED;
        VkExtent2D m_extent { };
        VkImageAspectFlags m_aspect = VK_IMAGE_ASPECT_COLOR_BIT;
        VkImageLayout m_layout = VK_IMAGE_LAYOUT_UNDEFINED;
        std::uint32_t m_mipLevels = 1;
        std::uint32_t m_arrayLayers = 1;
        bool m_ownsImage { };
    };
}
