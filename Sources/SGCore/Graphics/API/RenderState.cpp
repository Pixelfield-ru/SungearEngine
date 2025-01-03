//
// Created by stuka on 21.12.2024.
//

#include "RenderState.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "IFrameBuffer.h"

void SGCore::MeshRenderState::use(bool force) const noexcept
{
    CoreMain::getRenderer()->useMeshRenderState(*this, force);
}

void SGCore::BlendingState::use(bool force) const noexcept
{
    CoreMain::getRenderer()->useBlendingState(*this, force);
}

void SGCore::RenderState::use(bool force) const noexcept
{
    CoreMain::getRenderer()->useState(*this, force);
}
