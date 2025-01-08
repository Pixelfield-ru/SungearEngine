//
// Created by stuka on 07.01.2025.
//

#ifndef SUNGEARENGINE_MOTIONPLANNER_H
#define SUNGEARENGINE_MOTIONPLANNER_H

#include "MotionPlannerNode.h"
#include "SGCore/ECS/Component.h"
#include "SGCore/Memory/Assets/Skeleton.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"

namespace SGCore
{
    struct MotionPlanner : ECS::Component<MotionPlanner, const MotionPlanner>
    {
        MotionPlanner() noexcept;
        MotionPlanner(const MotionPlanner&) noexcept = default;
        MotionPlanner(MotionPlanner&&) noexcept = default;

        Ref<MotionPlannerNode> m_rootNode;
        AssetRef<Skeleton> m_skeleton;

        std::int32_t m_maxBonesPerMesh = 150;

        // this value shares between all children of entity that contains this component.
        // this buffer passes into mesh components of all children
        Ref<IUniformBuffer> m_bonesTransformationsUniformBuffer;

        [[nodiscard]] MotionPlanner copyStructure() const noexcept;

        [[nodiscard]] MotionPlanner& operator=(const MotionPlanner&) noexcept = default;
        [[nodiscard]] MotionPlanner& operator=(MotionPlanner&&) noexcept = default;
    };
}

#endif //SUNGEARENGINE_MOTIONPLANNER_H
