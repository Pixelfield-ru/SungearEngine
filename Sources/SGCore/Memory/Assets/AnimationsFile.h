//
// Created by stuka on 05.01.2025.
//

#ifndef SUNGEARENGINE_ANIMATIONSFILE_H
#define SUNGEARENGINE_ANIMATIONSFILE_H

#include "IAsset.h"
#include "SGCore/Memory/IAssetsRefsResolver.h"

#include <assimp/Importer.hpp>

namespace SGCore
{
    struct AnimationsFile : public IAsset
    {
        sg_serde_as_friend()

        sg_implement_type_id(AnimationsFile, 30)

        sg_assets_refs_resolver_as_friend

    private:
        // todo: make loading sungear formats
        void doLoad(const InterpolatedPath& path) override;

        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        void readFromExistingAssimpScene(const aiScene& scene) noexcept;
    };
}

#endif //SUNGEARENGINE_ANIMATIONSFILE_H
