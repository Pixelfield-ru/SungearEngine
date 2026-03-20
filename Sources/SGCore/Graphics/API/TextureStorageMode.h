//
// Created by stuka on 03.07.2025.
//

#pragma once

#include <cstdint>

namespace SGCore
{
    enum class TextureStorageMode : std::uint8_t
    {
        TSM_ONLY_GPU,
        TSM_RAM_AND_GPU
    };
}
