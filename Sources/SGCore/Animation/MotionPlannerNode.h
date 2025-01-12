//
// Created by stuka on 07.01.2025.
//

#ifndef SUNGEARENGINE_MOTIONPLANNERNODE_H
#define SUNGEARENGINE_MOTIONPLANNERNODE_H

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Memory/Assets/SkeletalAnimationAsset.h"

namespace SGCore
{
    struct MotionPlannerConnection;

    struct MotionPlannerNode
    {
        friend struct MotionPlannersResolver;

        std::vector<Ref<MotionPlannerConnection>> m_connections;

        AssetRef<SkeletalAnimationAsset> m_skeletalAnimation;

        // calls if:
        // 1) if connected to other node as child, then calls if
        // parent node is playing (m_isPlaying == true) and active (m_isActive == true)
        // 2) if not connected to other node as child (it means that this node is root),
        // then calls if this node is playing (m_isPlaying == true) and active (m_isActive == true)
        std::function<bool()> activationFunction = []() { return true; };

        /// m_isActive indicates whether the node is active. If the node is not active, then it and all its children are not animated.
        bool m_isActive = true;
        /// m_isPaused indicates whether the node animation has been paused. If true then m_isPlaying == false
        bool m_isPaused = false;
        /// m_isPlaying indicates whether a node animation is currently being played.
        bool m_isPlaying = true;
        ///m_isRepeated notifies whether the node animation will be repeated after the animation is completed.
        bool m_isRepeated = false;

        float m_animationSpeed = 1.0f;

        float m_currentAnimationTime = 0.0f;

        [[nodiscard]] MotionPlannerNode copyStructure() const noexcept;

    private:
        // uses to interpolate between two animations when connection is activated
        float m_currentBlendFactor = 0.0f;
    };
}

#endif //SUNGEARENGINE_MOTIONPLANNERNODE_H
