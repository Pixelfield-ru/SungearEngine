//
// Created by stuka on 25.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_TEXTURE2DASSET_H
#define SUNGEARENGINE_TEXTURE2DASSET_H

#include "IAsset.h"
#include "SGCore/Graphics/API/ITexture2D.h"

namespace Core::Graphics
{
    class ITexture2D;
}

namespace Core::Memory::Assets
{
    class Texture2DAsset : public IAsset, public std::enable_shared_from_this<Texture2DAsset>
    {
    private:
        SGGColorInternalFormat m_internalFormat = SGGColorInternalFormat::SGG_RGBA8;
        SGGColorFormat m_format = SGGColorFormat::SGG_RGBA;
        int m_width = 0;
        int m_height = 0;
        int m_channelsInFile = 0;

        std::shared_ptr<Graphics::ITexture2D> m_texture2D;
        std::shared_ptr<std::uint8_t[]> m_textureData;

    public:
        //Texture2DAsset() = default;

        [[nodiscard]] std::shared_ptr<IAsset> load(const std::string& path) override;

        std::shared_ptr<Graphics::ITexture2D> getTexture2D() noexcept;

        #pragma region Getters
        SGGColorInternalFormat getInternalFormat() noexcept;
        SGGColorFormat getFormat() noexcept;

        int getWidth() noexcept;
        int getHeight() noexcept;

        [[nodiscard]] std::shared_ptr<std::uint8_t[]> getData() noexcept;
        #pragma endregion
    };
}

#endif //SUNGEARENGINE_TEXTURE2DASSET_H
