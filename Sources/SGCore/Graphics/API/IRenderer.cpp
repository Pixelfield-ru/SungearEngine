#include "IRenderer.h"

Core::Graphics::APIType Core::Graphics::IRenderer::getAPIType() const noexcept
{
    return m_apiType;
}
