//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_ITEXTURE2D_H
#define NATIVECORE_ITEXTURE2D_H

#include <iostream>
#include "GraphicsDataTypes.h"
#include "SGCore/Memory/Assets/IAsset.h"
#include "SGCore/Memory/Assets/IAssetObserver.h"
#include "SGCore/Memory/Assets/Texture2DAsset.h"

namespace Core::Memory::Assets
{
    class Texture2DAsset;
}

namespace Core::Graphics::API
{
    class ITexture2D : public Memory::Assets::IAssetObserver
    {
    protected:
        std::weak_ptr<Memory::Assets::Texture2DAsset> m_texture2DAsset;

    public:
        virtual ~ITexture2D() = default;

        virtual void create(std::weak_ptr<Memory::Assets::Texture2DAsset>) = 0;
        virtual void destroy() = 0;

        virtual void bind() = 0;
    };
}

#endif //NATIVECORE_ITEXTURE2D_H
