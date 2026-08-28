//
// Created by 8bitniksis on 20.08.2026.
//

#include "DX12Shader.h"

#if defined(_WIN32)

#include "DX12Renderer.h"

void SGCore::DX12Shader::setAsCurrentShader() const noexcept
{
    if(const auto& renderer = DX12Renderer::getInstance())
    {
        renderer->setCurrentLegacyShader(const_cast<DX12Shader*>(this));
    }
}

#endif
