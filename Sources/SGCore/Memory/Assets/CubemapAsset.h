#ifndef SUNGEARENGINE_CUBEMAPASSET_H
#define SUNGEARENGINE_CUBEMAPASSET_H

#include "SGCore/Graphics/API/ICubemapTexture.h"

namespace SGCore
{
    class ICubemapTexture;

    class CubemapAsset : public Texture2DAsset
    {
    public:
        CubemapAsset(
                Ref<Texture2DAsset> xPositive, Ref<Texture2DAsset> xNegative,
                Ref<Texture2DAsset> yPositive, Ref<Texture2DAsset> yNegative,
                Ref<Texture2DAsset> zPositive, Ref<Texture2DAsset> zNegative
                );

        [[nodiscard]] Ref<IAsset> load(const std::string& path) override;

        std::list<Ref<Texture2DAsset>> getParts() noexcept;

    private:
        std::list<Ref<Texture2DAsset>> m_parts;
        Ref<ICubemapTexture> m_cubemapTexture;
    };
}

#endif //SUNGEARENGINE_CUBEMAPASSET_H
