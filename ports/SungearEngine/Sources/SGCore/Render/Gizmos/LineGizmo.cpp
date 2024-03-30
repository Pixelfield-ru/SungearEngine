//
// Created by stuka on 02.02.2024.
//

#include "LineGizmo.h"
#include "SGCore/Render/ShadersUtils.h"

SGCore::LineGizmo::LineGizmo() noexcept
{
    m_base.m_meshBase.m_meshDataRenderInfo.m_useIndices = false;
}