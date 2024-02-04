//
// Created by stuka on 02.02.2024.
//

#include "SphereGizmosUpdater.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/MeshBuilder.h"
#include "SphereGizmo.h"

void SGCore::SphereGizmosUpdater::fixedUpdate(const SGCore::Ref<SGCore::Scene>& scene)
{
    auto sphereGizmosView = scene->getECSRegistry().view<SphereGizmo>();

    sphereGizmosView.each([](SphereGizmo& gizmo) {
        if(gizmo.m_radius != gizmo.m_lastRadius || gizmo.m_angleIncrement != gizmo.m_lastAngleIncrement)
        {
            MeshBuilder::buildSphereVariant1(gizmo.m_base.m_meshBase, gizmo.m_radius, gizmo.m_angleIncrement);

            gizmo.m_lastRadius = gizmo.m_radius;
            gizmo.m_lastAngleIncrement = gizmo.m_angleIncrement;
        }
    });
}
