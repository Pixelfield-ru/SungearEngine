//
// Created by stuka on 05.01.2025.
//

#ifndef SUNGEARENGINE_SKELETON_H
#define SUNGEARENGINE_SKELETON_H

#include "IAsset.h"
#include "SGCore/Memory/AssetRefFromThis.h"
#include "SGCore/Memory/IAssetsRefsResolver.h"

namespace SGCore
{
    struct Bone;

    struct Skeleton : public IAsset, public IAssetsRefsResolver<Skeleton>, public AssetRefFromThis<Skeleton>
    {
        sg_serde_as_friend()

        sg_implement_type_id(Skeleton, 31)

        sg_assets_refs_resolver_as_friend

        // std::vector<Ref<Bone>> m_bones;

        void addBone(AssetRef<Bone> bone, std::string boneName) noexcept;
        // todo: impl
        void removeBone(const std::string& name) noexcept;

        void setRootBone(AssetRef<Bone> rootBone, std::string boneName) noexcept;
        const Bone* getRootBone() const noexcept;

        void renameBone(const std::string& lastBoneName, const std::string& newBoneName) noexcept;

        [[nodiscard]] Bone* findBone(const std::string& boneName) const noexcept;

        // const std::vector<AssetRef<Bone>>& getAllBones() const noexcept;
        std::int64_t bonesCount() const noexcept;

    protected:
        AssetRef<Bone> m_rootBone;
        std::vector<AssetRef<Bone>> m_allBones;
        // first - name of bone. second - index of bone in m_allBones vector.
        std::unordered_map<std::string, std::int64_t> m_allBonesMap;

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
