#ifndef SUNGEARENGINE_CUBEMAPASSET_H
#define SUNGEARENGINE_CUBEMAPASSET_H

#include "SGCore/Graphics/API/ICubemapTexture.h"

namespace Core::Graphics
{
    class ICubemapTexture;
}

namespace Core::Memory::Assets
{
    class CubemapAsset : public Texture2DAsset
    {
    public:
        CubemapAsset(
                std::shared_ptr<Texture2DAsset> xPositive, std::shared_ptr<Texture2DAsset> xNegative,
                std::shared_ptr<Texture2DAsset> yPositive, std::shared_ptr<Texture2DAsset> yNegative,
                std::shared_ptr<Texture2DAsset> zPositive, std::shared_ptr<Texture2DAsset> zNegative
                );

        [[nodiscard]] std::shared_ptr<IAsset> load(const std::string& path) override;

        std::list<std::shared_ptr<Texture2DAsset>> getParts() noexcept;

    private:
        std::list<std::shared_ptr<Texture2DAsset>> m_parts;
        std::shared_ptr<Graphics::ICubemapTexture> m_cubemapTexture;
    };
}

#endif //SUNGEARENGINE_CUBEMAPASSET_H
