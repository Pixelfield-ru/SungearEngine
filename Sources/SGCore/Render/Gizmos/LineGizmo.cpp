//
// Created by stuka on 02.02.2024.
//

#include "LineGizmo.h"
#include "SGCore/Render/MeshesUtils.h"

SGCore::LineGizmo::LineGizmo() noexcept
{
    MeshesUtils::loadMeshShader(m_base.m_meshBase, "Gizmos/LinesGizmosShader");
    
    m_base.m_meshBase.m_meshDataRenderInfo.m_useIndices = false;
}