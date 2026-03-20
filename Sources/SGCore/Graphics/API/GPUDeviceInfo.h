//
// Created by stuka on 03.07.2025.
//

#pragma once

#include <glm/vec2.hpp>

#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Main/CoreMain.h"

namespace SGCore
{
    struct SGCORE_EXPORT GPUDeviceInfo
    {
        static glm::ivec2 getMaxTextureSize() noexcept;
        static std::int32_t getMaxTextureBufferSize() noexcept;
        static std::int32_t getMaxVertexAttribsCount() noexcept;
    };
}
