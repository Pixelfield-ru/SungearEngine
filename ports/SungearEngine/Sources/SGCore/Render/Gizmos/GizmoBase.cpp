//
// Created by stuka on 02.02.2024.
//

#include "GizmoBase.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"

SGCore::GizmoBase::GizmoBase() noexcept
{
    m_meshBase.m_meshDataRenderInfo.m_enableFacesCulling = false;
    m_meshBase.m_meshDataRenderInfo.m_drawMode = SGDrawMode::SGG_LINES;
    m_meshBase.m_meshDataRenderInfo.m_linesWidth = 3.0f;
}
