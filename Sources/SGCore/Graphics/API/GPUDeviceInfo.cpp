//
// Created by stuka on 03.07.2025.
//

#include "GPUDeviceInfo.h"

#include "GL/DeviceGLInfo.h"
#include "SGCore/Graphics/RHI/IDevice.h"

glm::ivec2 SGCore::GPUDeviceInfo::getMaxTextureSize() noexcept
{
    switch(CoreMain::getRenderer()->getGAPIType())
    {
        case SG_API_TYPE_UNKNOWN: break;
        case SG_API_TYPE_GL4:
        {
            return { DeviceGLInfo::getMaxTextureSize(), DeviceGLInfo::getMaxTextureSize() };
        }
        case SG_API_TYPE_GL46:
        {
            return { DeviceGLInfo::getMaxTextureSize(), DeviceGLInfo::getMaxTextureSize() };
        }
        case SG_API_TYPE_GLES2:
        {
            return { DeviceGLInfo::getMaxTextureSize(), DeviceGLInfo::getMaxTextureSize() };
        }
        case SG_API_TYPE_GLES3:
        {
            return { DeviceGLInfo::getMaxTextureSize(), DeviceGLInfo::getMaxTextureSize() };
        }
        case SG_API_TYPE_VULKAN:
        case SG_API_TYPE_DX12: break;
    }

    return { };
}

std::int32_t SGCore::GPUDeviceInfo::getMaxTextureBufferSize() noexcept
{
    switch(CoreMain::getRenderer()->getGAPIType())
    {
        case SG_API_TYPE_UNKNOWN: break;
        case SG_API_TYPE_GL4:
        {
            return DeviceGLInfo::getMaxTextureBufferSize();
        }
        case SG_API_TYPE_GL46:
        {
            return DeviceGLInfo::getMaxTextureBufferSize();
        }
        case SG_API_TYPE_GLES2:
        {
            return DeviceGLInfo::getMaxTextureBufferSize();
        }
        case SG_API_TYPE_GLES3:
        {
            return DeviceGLInfo::getMaxTextureBufferSize();
        }
        // the explicit backends carry the limit in their RHI device properties; returning 0 here
        // made every "does this fit?" check fail, and batching rejected the first mesh it was given
        case SG_API_TYPE_VULKAN:
        case SG_API_TYPE_DX12:
        {
            const auto* device = CoreMain::getRenderer()->getDevice();
            if(device) return static_cast<std::int32_t>(device->getProperties().m_maxTexelBufferElements);
            break;
        }
    }

    return { };
}

std::int32_t SGCore::GPUDeviceInfo::getMaxVertexAttribsCount() noexcept
{
    switch(CoreMain::getRenderer()->getGAPIType())
    {
        case SG_API_TYPE_UNKNOWN: break;
        case SG_API_TYPE_GL4:
        {
            return DeviceGLInfo::getMaxVertexAttribsCount();
        }
        case SG_API_TYPE_GL46:
        {
            return DeviceGLInfo::getMaxVertexAttribsCount();
        }
        case SG_API_TYPE_GLES2:
        {
            return DeviceGLInfo::getMaxVertexAttribsCount();
        }
        case SG_API_TYPE_GLES3:
        {
            return DeviceGLInfo::getMaxVertexAttribsCount();
        }
        case SG_API_TYPE_VULKAN:
        case SG_API_TYPE_DX12: break;
    }

    return { };
}
