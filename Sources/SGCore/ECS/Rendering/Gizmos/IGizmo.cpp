//
// Created by stuka on 14.11.2023.
//
#include "IGizmo.h"

SGCore::IGizmo::IGizmo() noexcept
{
    m_meshDataRenderInfo.m_enableFacesCulling = false;
    m_meshDataRenderInfo.m_drawMode = SGDrawMode::SGG_LINES;
}

