//
// Created by stuka on 05.01.2025.
//

#ifndef SUNGEARENGINE_SKELETON_H
#define SUNGEARENGINE_SKELETON_H

#include "IAsset.h"
#include "SGCore/ImportedScenesArch/Bone.h"

namespace SGCore
{
    struct Skeleton : public IAsset, public IAssetsRefsResolver<Skeleton>
    {
        sg_serde_as_friend()

        sg_implement_type_id(Skeleton, 31)

        sg_assets_refs_resolver_as_friend

        AssetRef<Bone> m_rootBone;
        std::vector<AssetRef<Bone>> m_allBones;
        // std::vector<Ref<Bone>> m_bones;

        [[nodiscard]] AssetRef<Bone> findBone(const std::string& boneName) const noexcept;

    protected:
        /// does nothing for now
        void doLoad(const InterpolatedPath& path) override;

        /// does nothing!!!
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        /// does nothing for now
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept SG_CRTP_OVERRIDE;
    };
}

#endif //SUNGEARENGINE_SKELETON_H
