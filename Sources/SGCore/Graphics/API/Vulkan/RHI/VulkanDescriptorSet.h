//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <map>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/IDescriptorSet.h"
#include "VulkanContext.h"

namespace SGCore
{
    class VulkanDevice;
    class VulkanShaderProgram;
    class VulkanTexture;

    /// Binding table on the CPU side. Bound to a command list it materializes into a transient
    /// VkDescriptorSet matching the current pipeline's layout (see VulkanCommandList::flushState()).
    class SGCORE_EXPORT VulkanDescriptorSet final : public IDescriptorSet
    {
    public:
        struct Entry
        {
            Ref<IGPUBuffer> m_buffer;
            std::uint64_t m_offset { };
            std::uint64_t m_range { };
            bool m_storage { };
            Ref<ITexture2D> m_texture;
            Ref<ICubemapTexture> m_cubemap;
            /// Set when the caller already holds the backend texture (VkShader resolving texture
            /// units); takes precedence over m_texture, which needs a VkTexture2D to unwrap.
            Ref<VulkanTexture> m_vulkanTexture;
            /// Uniform texel buffer (a samplerBuffer in GLSL); the set does not own the view.
            VkBufferView m_texelBufferView = VK_NULL_HANDLE;
            std::uint32_t m_arrayIndex { };
        };

        explicit VulkanDescriptorSet(VulkanDevice& device) noexcept : m_device(device) { }

        void setUniformBuffer(std::uint32_t binding, const Ref<IGPUBuffer>& buffer,
                              std::uint64_t offset = 0, std::uint64_t range = 0) noexcept override;
        void setStorageBuffer(std::uint32_t binding, const Ref<IGPUBuffer>& buffer,
                              std::uint64_t offset = 0, std::uint64_t range = 0) noexcept override;
        void setTexture(std::uint32_t binding, const Ref<ITexture2D>& texture, std::uint32_t arrayIndex = 0) noexcept override;
        void setCubemap(std::uint32_t binding, const Ref<ICubemapTexture>& texture) noexcept override;

        void setBackendTexture(std::uint32_t binding, const Ref<IGPUObject>& texture, std::uint32_t arrayIndex = 0) noexcept override;

        /// Vulkan-only overload: binds a backend texture without going through a legacy facade.
        void setVulkanTexture(std::uint32_t binding, const Ref<VulkanTexture>& texture, std::uint32_t arrayIndex = 0) noexcept;

        /// Vulkan-only: binds a uniform texel buffer view (GLSL samplerBuffer).
        void setTexelBuffer(std::uint32_t binding, VkBufferView view, std::uint32_t arrayIndex = 0) noexcept;

        /// (binding, arrayIndex) -> entry
        [[nodiscard]] const std::map<std::pair<std::uint32_t, std::uint32_t>, Entry>& getEntries() const noexcept { return m_entries; }
        [[nodiscard]] std::uint64_t getVersion() const noexcept { return m_version; }

    private:
        VulkanDevice& m_device;
        std::map<std::pair<std::uint32_t, std::uint32_t>, Entry> m_entries;
        std::uint64_t m_version { };
    };
}
