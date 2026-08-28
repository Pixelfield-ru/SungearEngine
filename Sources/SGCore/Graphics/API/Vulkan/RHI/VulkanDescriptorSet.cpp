//
// Created by 8bitniksis on 17.08.2026.
//

#include "VulkanDescriptorSet.h"

#include "VulkanGPUBuffer.h"

#include "VulkanTexture.h"

void SGCore::VulkanDescriptorSet::setUniformBuffer(std::uint32_t binding, const Ref<IGPUBuffer>& buffer,
                                                   std::uint64_t offset, std::uint64_t range) noexcept
{
    auto& entry = m_entries[{ binding, 0 }];
    entry = { };
    entry.m_buffer = buffer;
    entry.m_offset = offset;
    entry.m_range = range;
    entry.m_storage = false;
    ++m_version;
}

void SGCore::VulkanDescriptorSet::setStorageBuffer(std::uint32_t binding, const Ref<IGPUBuffer>& buffer,
                                                   std::uint64_t offset, std::uint64_t range) noexcept
{
    auto& entry = m_entries[{ binding, 0 }];
    entry = { };
    entry.m_buffer = buffer;
    entry.m_offset = offset;
    entry.m_range = range;
    entry.m_storage = true;
    ++m_version;
}

void SGCore::VulkanDescriptorSet::setTexture(std::uint32_t binding, const Ref<ITexture2D>& texture, std::uint32_t arrayIndex) noexcept
{
    auto& entry = m_entries[{ binding, arrayIndex }];
    entry = { };
    entry.m_texture = texture;
    entry.m_arrayIndex = arrayIndex;
    ++m_version;
}

void SGCore::VulkanDescriptorSet::setBackendTexture(std::uint32_t binding, const Ref<IGPUObject>& texture, std::uint32_t arrayIndex) noexcept
{
    // The unit table keeps whatever the backend put there as IGPUObject. Usually a VulkanTexture,
    // but a texture of type SG_TEXTURE_BUFFER is a texel buffer instead — a different descriptor
    // kind, so it can not go through the image path.
    if(const auto buffer = std::dynamic_pointer_cast<VulkanGPUBuffer>(texture))
    {
        setTexelBuffer(binding, buffer->getTexelView(), arrayIndex);
        return;
    }

    setVulkanTexture(binding, std::dynamic_pointer_cast<VulkanTexture>(texture), arrayIndex);
}

void SGCore::VulkanDescriptorSet::setVulkanTexture(std::uint32_t binding, const Ref<VulkanTexture>& texture, std::uint32_t arrayIndex) noexcept
{
    auto& entry = m_entries[{ binding, arrayIndex }];
    // the same texture in the same slot is the common case (a pass re-binding every frame):
    // do not bump the version, or every draw would allocate a fresh descriptor set
    if(entry.m_vulkanTexture == texture && !entry.m_texture && !entry.m_buffer) return;

    entry = { };
    entry.m_vulkanTexture = texture;
    entry.m_arrayIndex = arrayIndex;
    ++m_version;
}

void SGCore::VulkanDescriptorSet::setTexelBuffer(std::uint32_t binding, VkBufferView view, std::uint32_t arrayIndex) noexcept
{
    auto& entry = m_entries[{ binding, arrayIndex }];
    // re-binding the same view every frame must not invalidate the set (see setVulkanTexture)
    if(entry.m_texelBufferView == view && !entry.m_buffer && !entry.m_texture && !entry.m_vulkanTexture) return;

    entry = { };
    entry.m_texelBufferView = view;
    entry.m_arrayIndex = arrayIndex;
    ++m_version;
}

void SGCore::VulkanDescriptorSet::setCubemap(std::uint32_t binding, const Ref<ICubemapTexture>& texture) noexcept
{
    auto& entry = m_entries[{ binding, 0 }];
    entry = { };
    entry.m_cubemap = texture;
    ++m_version;
}
