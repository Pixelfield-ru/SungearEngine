//
// Created by stuka on 05.01.2025.
//

#ifndef SUNGEARENGINE_SKELETALANIMATIONASSET_H
#define SUNGEARENGINE_SKELETALANIMATIONASSET_H

#include "IAsset.h"
#include "SGCore/Memory/IAssetsRefsResolver.h"

namespace SGCore
{
    struct KeyPosition
    {
        glm::vec3 m_position { };
        float m_timeStamp = 0.0f;
    };

    struct KeyRotation
    {
        glm::quat m_rotation = glm::identity<glm::quat>();
        float m_timeStamp = 0.0f;
    };

    struct KeyScale
    {
        glm::vec3 m_scale { };
        float m_timeStamp = 0.0f;
    };

    struct SkeletalBoneAnimation
    {
        std::string m_boneName;
        std::vector<KeyPosition> m_positionKeys;
        std::vector<KeyRotation> m_rotationKeys;
        std::vector<KeyScale> m_scaleKeys;
    };

    struct SkeletalAnimationAsset : public IAsset
    {
        sg_serde_as_friend()

        sg_implement_type_id(SkeletalAnimationAsset, 29)

        // sg_assets_refs_resolver_as_friend

        std::string m_animationName;

        std::vector<SkeletalBoneAnimation> m_bonesAnimations;

    private:
        /// DOES NOTHING!
        void doLoad(const InterpolatedPath& path) override;

        // todo: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        /// DOES NOTHING!
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;
    };
}

#endif //SUNGEARENGINE_SKELETALANIMATIONASSET_H
