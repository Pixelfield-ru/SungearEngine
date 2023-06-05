//
// Created by stuka on 05.06.2023.
//

#include "GLShaderUniform.h"

void Core::Graphics::API::GL::GLShaderUniform::updateLocation(const std::shared_ptr<Core::Graphics::API::IShader>& fromShader) noexcept
{
    m_location =  fromShader->getShaderUniformLocation(m_name);
}
