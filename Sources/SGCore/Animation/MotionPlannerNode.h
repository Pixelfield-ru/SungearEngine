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
        std::vector<Ref<MotionPlannerConnection>> m_connections;

        AssetRef<SkeletalAnimationAsset> m_skeletalAnimation;

        bool m_isActive = false;
        bool m_isRepeated = false;

        float m_animationSpeed = 1.0f;

        float m_currentAnimationTime = 0.0f;

        [[nodiscard]] MotionPlannerNode copyStructure() const noexcept;
    };
}

#endif //SUNGEARENGINE_MOTIONPLANNERNODE_H
