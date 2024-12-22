#include "IRenderer.h"

SGCore::GAPIType SGCore::IRenderer::getGAPIType() const noexcept
{
    return m_apiType;
}

const SGCore::RenderState& SGCore::IRenderer::getCachedRenderState() const noexcept
{
    return m_cachedRenderState;
}
