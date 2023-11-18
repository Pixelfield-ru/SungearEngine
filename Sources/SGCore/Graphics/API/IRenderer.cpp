#include "IRenderer.h"

Core::Graphics::GAPIType Core::Graphics::IRenderer::getGAPIType() const noexcept
{
    return m_apiType;
}
