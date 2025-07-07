//
// Created by stuka on 03.07.2025.
//

#include "GPUDeviceInfo.h"

#include "GL/DeviceGLInfo.h"

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
        case SG_API_TYPE_VULKAN: break;
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
        case SG_API_TYPE_VULKAN: break;
    }

    return { };
}
