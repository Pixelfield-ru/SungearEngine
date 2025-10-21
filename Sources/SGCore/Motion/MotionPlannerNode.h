//
// Created by stuka on 07.01.2025.
//

#ifndef SUNGEARENGINE_MOTIONPLANNERNODE_H
#define SUNGEARENGINE_MOTIONPLANNERNODE_H

#include "MotionAnyState.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Memory/Assets/SkeletalAnimationAsset.h"
#include "SGCore/Actions/IAction.h"
#include "SGCore/Actions/AlwaysTrueAction.h"

namespace SGCore
{
    struct MotionPlannerConnection;

    /**
     * YOU CAN SERIALIZE ONLY Ref<MotionPlannerNode> (Ref is alias for std::shared_ptr).
     */
    struct MotionPlannerNode : IAssetsRefsResolver<MotionPlannerNode>, public std::enable_shared_from_this<MotionPlannerNode>
    {
        sg_serde_as_friend()
        sg_assets_refs_resolver_as_friend

        friend struct MotionPlannersResolver;

        std::vector<Ref<MotionPlannerConnection>> m_connections;

        AssetRef<SkeletalAnimationAsset> m_skeletalAnimation;

        // used as 'any state' to interpolate from any state to root nodes
        MotionAnyState m_anyState;

        /// m_isActive indicates whether the node is active. If the node is not active, then it and all its children are not animated.
        bool m_isActive = true;
        /// m_isPaused indicates whether the node animation has been paused. If true then m_isPlaying == false
        bool m_isPaused = false;
        /// m_isPlaying indicates whether a node animation is currently being played.
        bool m_isPlaying = true;
        /// m_isRepeated notifies whether the node animation will be repeated after the animation is completed.
        bool m_isRepeated = false;

        float m_animationSpeed = 1.0f;

        float m_currentAnimationTime = 0.0f;

        /// Minimum blend factor. Helpful for blending root nodes.
        float m_staticBlendFactor = 0.0f;

        /// Called when animation ends.
        Ref<IAction<void(MotionPlannerNode& thisNode)>> onEnd;

        [[nodiscard]] Ref<MotionPlannerNode> copyStructure() const noexcept;

        [[nodiscard]] SGCORE_EXPORT static Ref<MotionPlannerNode> createNode() noexcept;

    private:
        // uses to interpolate between two animations when connection is activated
        float m_currentBlendFactor = 0.0f;

        MotionPlannerNode() noexcept = default;
        MotionPlannerNode(const MotionPlannerNode&) noexcept = default;
        MotionPlannerNode(MotionPlannerNode&&) noexcept = default;

        MotionPlannerNode& operator=(const MotionPlannerNode&) noexcept = default;
        MotionPlannerNode& operator=(MotionPlannerNode&&) noexcept = default;

        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept;
    };
}

#endif //SUNGEARENGINE_MOTIONPLANNERNODE_H
