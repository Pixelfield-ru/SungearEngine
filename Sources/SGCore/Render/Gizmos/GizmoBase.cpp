//
// Created by stuka on 02.02.2024.
//

#include "GizmoBase.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"

SGCore::GizmoBase::GizmoBase() noexcept
{
    m_renderState.m_useFacesCulling = false;
    m_renderState.m_drawMode = SGDrawMode::SGG_LINES;
    m_renderState.m_linesWidth = 3.0f;
}
