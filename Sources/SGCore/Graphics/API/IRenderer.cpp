#include "IRenderer.h"

SGCore::GAPIType SGCore::IRenderer::getGAPIType() const noexcept
{
    return m_apiType;
}

SGCore::RenderState& SGCore::IRenderer::getCachedRenderState() noexcept
{
    return m_cachedRenderState;
}
