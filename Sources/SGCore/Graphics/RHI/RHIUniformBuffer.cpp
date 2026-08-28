//
// Created by stuka on 07.07.2023.
//

#include "RHIUniformBuffer.h"

#include "IDevice.h"
#include "LiveDevice.h"
#include "SharedUniformBuffers.h"
#include "SGCore/Logger/Logger.h"


namespace
{
    SGCore::IDevice* currentDevice() noexcept
    {
        return SGCore::LiveDevice::get();
    }
}

SGCore::RHIUniformBuffer::~RHIUniformBuffer()
{
    RHIUniformBuffer::destroy();
}

void SGCore::RHIUniformBuffer::prepare() noexcept
{
    destroy();

    auto* device = currentDevice();
    if(!device || m_bufferSize <= 0) return;

    GPUBufferDesc desc;
    desc.m_size = static_cast<std::uint64_t>(m_bufferSize);
    desc.m_usage = GPUBufferUsage::SGG_UNIFORM_BUFFER;
    // rewritten every frame (camera matrices, time): host-visible, no staging round trip
    desc.m_access = GPUMemoryAccess::SGG_HOST_VISIBLE;
    desc.m_debugName = m_blockName.empty() ? "shared_ubo" : m_blockName;
    m_buffer_gpu = device->createBuffer(desc);
    if(!m_buffer_gpu) return;

    if(m_buffer) m_buffer_gpu->write(m_buffer, static_cast<std::uint64_t>(m_bufferSize), 0);

    if(m_blockName.empty())
    {
        SG_LOG_W("RHIUniformBuffer without a block name: shaders can not find it (bindings are assigned by block name here).");
        return;
    }
    SharedUniformBuffers::set(m_blockName, m_buffer_gpu);
}

void SGCore::RHIUniformBuffer::subDataOnGAPISide(const std::int64_t& offset, const int& size) noexcept
{
    if(!m_buffer_gpu || !m_buffer || size <= 0) return;
    m_buffer_gpu->write(m_buffer + offset, static_cast<std::uint64_t>(size), static_cast<std::uint64_t>(offset));
}

void SGCore::RHIUniformBuffer::bind() noexcept
{
    // nothing to bind: shaders pick the buffer up by block name when they build their descriptor set
}

void SGCore::RHIUniformBuffer::setLayoutLocation(const std::uint16_t& location) noexcept
{
    m_layoutLocation = location;
}

void SGCore::RHIUniformBuffer::destroy() noexcept
{
    if(!m_buffer_gpu) return;

    if(!m_blockName.empty() && SharedUniformBuffers::get(m_blockName) == m_buffer_gpu)
    {
        SharedUniformBuffers::remove(m_blockName);
    }
    if(auto* device = currentDevice()) device->destroyDeferred(m_buffer_gpu);
    m_buffer_gpu = nullptr;
}
