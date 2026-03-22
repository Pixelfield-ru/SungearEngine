//
// Created by stuka on 07.01.2025.
//

#pragma once

#include "SGCore/ECS/Component.h"
#include "SGCore/Memory/Assets/Skeleton.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Graphics/API/ITexture2D.h"

namespace SGCore
{
    /**
     * MotionPlannersResolver is required system to resolve skeletal animations nodes, 3D ragdolls and IK.\n
     * If you want animations, then MotionPlanner must be added on the root model entity.
     */
    struct SGCORE_EXPORT MotionPlanner : ECS::Component<MotionPlanner, const MotionPlanner>, IAssetsRefsResolver<MotionPlanner>
    {
        sg_assets_refs_resolver_as_friend

        MotionPlanner() noexcept;
        MotionPlanner(const MotionPlanner&) noexcept = default;
        MotionPlanner(MotionPlanner&&) noexcept = default;

        AssetRef<Skeleton> m_skeleton;

        std::int32_t m_maxBonesPerMesh = 300;

        /**
         * this value shares between all children of entity that contains this component.
         * this buffer passes into mesh components of all children
         * size of this buffer is m_maxBonesPerMesh * 16 + 4. + 4 because in first vec4 we store count of bones
         */
        Ref<ITexture2D> m_bonesMatricesBuffer;
        /**
         * we store this data to put all data of bones matrices in bones matrices texture buffer (m_bonesMatricesBuffer)
         * size of this data is m_maxBonesPerMesh * 16 + 4. + 4 because in first vec4 we store count of bones
         */
        std::vector<float> m_bonesMatricesData;

        [[nodiscard]] MotionPlanner copyStructure() const noexcept;
        /**
         *
         * @return
         */

        [[nodiscard]] MotionPlanner& operator=(const MotionPlanner&) noexcept = default;
        [[nodiscard]] MotionPlanner& operator=(MotionPlanner&&) noexcept = default;

    private:
        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept;
    };
}
