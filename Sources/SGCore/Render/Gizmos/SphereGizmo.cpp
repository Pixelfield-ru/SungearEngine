//
// Created by stuka on 02.02.2024.
//

#include "SphereGizmo.h"
#include "SGCore/Render/MeshBase.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Render/MeshesUtils.h"

SGCore::SphereGizmo::SphereGizmo() noexcept
{
    MeshesUtils::loadMeshShader(m_base.m_meshBase, "Gizmos/ComplexGizmosShader");
}
