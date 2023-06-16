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
    class ITexture2D : public Memory::Assets::IAssetObserver, public std::enable_shared_from_this<ITexture2D>
    {
    protected:
        std::weak_ptr<Memory::Assets::Texture2DAsset> m_texture2DAsset;
        std::uint8_t m_unit = 0;

    public:
        virtual ~ITexture2D() = default;

        virtual void create(std::weak_ptr<Memory::Assets::Texture2DAsset>) = 0;
        virtual void destroy() = 0;

        virtual void bind() = 0;

        #pragma region Getters and setters

        virtual std::shared_ptr<ITexture2D> setUnit(const std::uint8_t& unit);
        virtual std::uint8_t getUnit() noexcept;

        std::weak_ptr<Memory::Assets::Texture2DAsset> getAsset() noexcept;

        #pragma endregion

        #pragma region Operators

        virtual std::shared_ptr<ITexture2D> operator=(const std::shared_ptr<ITexture2D>& other) = 0;
        //virtual operator=(std::shared_ptr<ITexture2D> other);

        #pragma endregion
    };
}

#endif //NATIVECORE_ITEXTURE2D_H
