//
// Created by stuka on 05.01.2025.
//

#ifndef SUNGEARENGINE_SKELETALANIMATIONASSET_H
#define SUNGEARENGINE_SKELETALANIMATIONASSET_H

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

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
        glm::vec3 m_scale = { 1.0, 1.0, 1.0 };
        float m_timeStamp = 0.0f;
    };

    struct SkeletalBoneAnimation
    {
        sg_serde_as_friend()

        friend struct AnimationsFile;
        friend struct SkeletalAnimationAsset;

        std::vector<KeyPosition> m_positionKeys;
        std::vector<KeyRotation> m_rotationKeys;
        std::vector<KeyScale> m_scaleKeys;

        void sortPositionKeys() noexcept;
        void sortRotationKeys() noexcept;
        void sortScaleKeys() noexcept;
        void sortAllKeys() noexcept;

        [[nodiscard]] std::int64_t findPositionKeyByTime(const float& time) const noexcept;
        [[nodiscard]] std::int64_t findRotationKeyByTime(const float& time) const noexcept;
        [[nodiscard]] std::int64_t findScaleKeyByTime(const float& time) const noexcept;

        [[nodiscard]] const std::string& getBoneName() const noexcept;

    private:
        AssetsPackage::DataMarkup m_positionKeysMarkupInPackage;
        AssetsPackage::DataMarkup m_rotationKeysMarkupInPackage;
        AssetsPackage::DataMarkup m_scaleKeysMarkupInPackage;

        std::string m_boneName;
    };

    struct SkeletalAnimationAsset : public IAsset
    {
        sg_serde_as_friend()

        sg_implement_type_id(SkeletalAnimationAsset, 29)

        sg_assets_refs_resolver_as_friend

        friend struct AnimationsFile;

        // sg_assets_refs_resolver_as_friend

        std::string m_animationName;

        float m_duration = 0.0f;
        float m_ticksPerSecond = 0.0f;

        void changeBoneName(const std::string& lastBoneName, const std::string& newBoneName) noexcept;
        SkeletalBoneAnimation* getBoneAnimation(const std::string& boneName) noexcept;

        size_t getBonesAnimationsCount() const noexcept;

    private:
        // first - bone name
        std::unordered_map<std::string, SkeletalBoneAnimation> m_bonesAnimations;

        /// DOES NOTHING!
        void doLoad(const InterpolatedPath& path) override;

        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        /// DOES NOTHING!
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;
    };
}

#endif //SUNGEARENGINE_SKELETALANIMATIONASSET_H
