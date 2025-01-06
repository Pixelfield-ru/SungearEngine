//
// Created by stuka on 05.01.2025.
//

#ifndef SUNGEARENGINE_ANIMATIONSFILE_H
#define SUNGEARENGINE_ANIMATIONSFILE_H

#include "IAsset.h"
#include "SGCore/Memory/IAssetsRefsResolver.h"
#include "SkeletalAnimationAsset.h"

#include <assimp/Importer.hpp>

namespace SGCore
{
    struct AnimationsFile : public IAsset, public IAssetsRefsResolver<AnimationsFile>
    {
        sg_serde_as_friend()

        sg_implement_type_id(AnimationsFile, 30)

        sg_assets_refs_resolver_as_friend

        friend struct ModelAsset;

        std::vector<AssetRef<SkeletalAnimationAsset>> m_skeletalAnimations;

    private:
        // todo: make loading sungear formats
        void doLoad(const InterpolatedPath& path) override;

        // todo: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        /// todo: impl
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        // todo: impl
        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept SG_CRTP_OVERRIDE;

        void readFromExistingAssimpScene(const aiScene& scene) noexcept;
    };
}

#endif //SUNGEARENGINE_ANIMATIONSFILE_H
