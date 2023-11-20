#include "IRenderer.h"

SGCore::GAPIType SGCore::IRenderer::getGAPIType() const noexcept
{
    return m_apiType;
}
