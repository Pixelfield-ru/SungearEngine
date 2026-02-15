//
// Created by stuka on 15.02.2026.
//

#pragma once

#include "SGCore/Memory/AssetRef.h"

namespace SGCore
{
    class ITexture2D;

    struct Frame
    {
        AssetRef<ITexture2D> m_texture;
        int m_nextFrameDelay {};
    };
}
