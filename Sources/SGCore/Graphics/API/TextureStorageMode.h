//
// Created by stuka on 03.07.2025.
//

#ifndef SUNGEARENGINE_TEXTURESTORAGEMODE_H
#define SUNGEARENGINE_TEXTURESTORAGEMODE_H

#include <cstdint>

namespace SGCore
{
    enum class TextureStorageMode : std::uint8_t
    {
        TSM_ONLY_GPU,
        TSM_RAM_AND_GPU
    };
}

#endif // SUNGEARENGINE_TEXTURESTORAGEMODE_H
