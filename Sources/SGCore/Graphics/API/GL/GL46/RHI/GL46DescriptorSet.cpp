//
// Created by 8bitniksis on 17.08.2026.
//

#include "GL46DescriptorSet.h"

#include <cstdint>

#include "GL46GPUBuffer.h"
#include "SGCore/Graphics/API/ICubemapTexture.h"
#include "SGCore/Graphics/API/ITexture2D.h"

void SGCore::GL46DescriptorSet::setBuffer(GLenum target, std::uint32_t binding, const Ref<IGPUBuffer>& buffer,
                                          std::uint64_t offset, std::uint64_t range) noexcept
{
    for(auto& entry : m_buffers)
    {
        if(entry.m_target == target && entry.m_binding == binding)
        {
            entry.m_buffer = buffer;
            entry.m_offset = offset;
            entry.m_range = range;
            return;
        }
    }
    m_buffers.push_back({ target, binding, buffer, offset, range });
}

void SGCore::GL46DescriptorSet::setUniformBuffer(std::uint32_t binding, const Ref<IGPUBuffer>& buffer, std::uint64_t offset, std::uint64_t range) noexcept
{
    setBuffer(GL_UNIFORM_BUFFER, binding, buffer, offset, range);
}

void SGCore::GL46DescriptorSet::setStorageBuffer(std::uint32_t binding, const Ref<IGPUBuffer>& buffer, std::uint64_t offset, std::uint64_t range) noexcept
{
    setBuffer(GL_SHADER_STORAGE_BUFFER, binding, buffer, offset, range);
}

void SGCore::GL46DescriptorSet::setTexture(std::uint32_t binding, const Ref<ITexture2D>& texture, std::uint32_t arrayIndex) noexcept
{
    // arrays of samplers occupy consecutive units starting at the binding
    const std::uint32_t unit = binding + arrayIndex;
    const GLuint handle = texture ? static_cast<GLuint>(reinterpret_cast<std::intptr_t>(texture->getTextureNativeHandler())) : 0;

    for(auto& entry : m_textures)
    {
        if(entry.m_unit == unit)
        {
            entry.m_handle = handle;
            entry.m_texture2D = texture;
            entry.m_cubemap = nullptr;
            return;
        }
    }
    m_textures.push_back({ unit, handle, texture, nullptr });
}

void SGCore::GL46DescriptorSet::setCubemap(std::uint32_t binding, const Ref<ICubemapTexture>& texture) noexcept
{
    const GLuint handle = texture ? static_cast<GLuint>(reinterpret_cast<std::intptr_t>(texture->getTextureNativeHandler())) : 0;

    for(auto& entry : m_textures)
    {
        if(entry.m_unit == binding)
        {
            entry.m_handle = handle;
            entry.m_texture2D = nullptr;
            entry.m_cubemap = texture;
            return;
        }
    }
    m_textures.push_back({ binding, handle, nullptr, texture });
}

void SGCore::GL46DescriptorSet::apply() const noexcept
{
    for(const auto& entry : m_buffers)
    {
        const auto* buffer = static_cast<const GL46GPUBuffer*>(entry.m_buffer.get());
        if(!buffer) continue;

        if(entry.m_range == 0 && entry.m_offset == 0)
        {
            glBindBufferBase(entry.m_target, entry.m_binding, buffer->getHandle());
        }
        else
        {
            const std::uint64_t range = entry.m_range == 0 ? buffer->getDesc().m_size - entry.m_offset : entry.m_range;
            glBindBufferRange(entry.m_target, entry.m_binding, buffer->getHandle(),
                              static_cast<GLintptr>(entry.m_offset), static_cast<GLsizeiptr>(range));
        }
    }

    for(const auto& entry : m_textures)
    {
        glBindTextureUnit(entry.m_unit, entry.m_handle);
    }
}
