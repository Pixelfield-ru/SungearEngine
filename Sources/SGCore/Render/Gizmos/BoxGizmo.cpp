#include "BoxGizmo.h"

#include "SGCore/Main/CoreMain.h"

SGCore::BoxGizmo::BoxGizmo() noexcept
{
    MeshesUtils::loadMeshShader(m_base.m_meshBase, "Gizmos/ComplexGizmosShader");
}