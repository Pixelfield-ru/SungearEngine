//
// Created by stuka on 03.07.2025.
//

#ifndef SUNGEARENGINE_GPUDEVICEINFO_H
#define SUNGEARENGINE_GPUDEVICEINFO_H

#include <glm/vec2.hpp>

#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Main/CoreMain.h"

namespace SGCore
{
    struct GPUDeviceInfo
    {
        static glm::ivec2 getMaxTextureSize() noexcept;
        static std::int32_t getMaxTextureBufferSize() noexcept;
    };
}

#endif // SUNGEARENGINE_GPUDEVICEINFO_H
