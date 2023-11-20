//
// Created by stuka on 25.05.2023.
//

#ifndef SUNGEARENGINE_TEXTURE2DASSET_H
#define SUNGEARENGINE_TEXTURE2DASSET_H

#include "IAsset.h"
#include "SGCore/Graphics/API/ITexture2D.h"

namespace SGCore
{
    class ITexture2D;

    struct Texture2DDataDeleter
    {
        void operator()(std::uint8_t* data);
    };

    class Texture2DAsset : public IAsset, public std::enable_shared_from_this<Texture2DAsset>
    {
    public:
        std::shared_ptr<ITexture2D> m_texture2D;

        Texture2DAsset() = default;

        [[nodiscard]] std::shared_ptr<IAsset> load(const std::string& path) override;

        virtual std::shared_ptr<ITexture2D> getTexture2D() noexcept;

        #pragma region Getters
        virtual SGGColorInternalFormat getInternalFormat() const noexcept;
        virtual SGGColorFormat getFormat() const noexcept;

        virtual int getWidth() noexcept;
        virtual int getHeight() noexcept;

        [[nodiscard]] std::shared_ptr<std::uint8_t[]> getData() noexcept;
        #pragma endregion

    protected:
        SGGColorInternalFormat m_internalFormat = SGGColorInternalFormat::SGG_RGBA8;
        SGGColorFormat m_format = SGGColorFormat::SGG_RGBA;

        int m_width = 0;
        int m_height = 0;
        int m_channelsInFile = 0;

    private:
        std::shared_ptr<std::uint8_t[]> m_textureData;
    };
}

#endif //SUNGEARENGINE_TEXTURE2DASSET_H
