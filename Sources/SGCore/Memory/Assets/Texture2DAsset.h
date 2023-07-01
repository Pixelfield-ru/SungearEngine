//
// Created by stuka on 25.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_TEXTURE2DASSET_H
#define SUNGEARENGINE_TEXTURE2DASSET_H

#include "IAsset.h"
#include "SGCore/Graphics/API/ITexture2D.h"

namespace Core::Graphics::API
{
    class ITexture2D;
}

namespace Core::Memory::Assets
{
    class Texture2DAsset : public IAsset, public std::enable_shared_from_this<Texture2DAsset>
    {
    private:
        SGGInternalFormat m_internalFormat = SGGInternalFormat::SGG_RGBA8;
        SGGFormat m_format = SGGFormat::SGG_RGBA;
        int m_width = 0;
        int m_height = 0;
        int m_channelsInFile = 0;

        std::shared_ptr<Graphics::API::ITexture2D> m_texture2D;
        std::shared_ptr<std::uint8_t[]> m_textureData;

    public:
        //Texture2DAsset() = default;

        [[nodiscard]] std::shared_ptr<IAsset> load(const std::string& path) override;

        std::shared_ptr<Graphics::API::ITexture2D> getTexture2D() noexcept;

        #pragma region Getters
        SGGInternalFormat getInternalFormat() noexcept;
        SGGFormat getFormat() noexcept;

        int getWidth() noexcept;
        int getHeight() noexcept;

        [[nodiscard]] std::shared_ptr<std::uint8_t[]> getData() noexcept;
        #pragma endregion
    };
}

#endif //SUNGEARENGINE_TEXTURE2DASSET_H
