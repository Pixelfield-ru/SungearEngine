//
// Created by stuka on 02.02.2024.
//

#include "GizmoBase.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"

SGCore::GizmoBase::GizmoBase() noexcept
{
    m_meshRenderState.m_useFacesCulling = false;
    m_meshRenderState.m_drawMode = SGDrawMode::SGG_LINES;
    m_meshRenderState.m_linesWidth = 3.0f;
}
