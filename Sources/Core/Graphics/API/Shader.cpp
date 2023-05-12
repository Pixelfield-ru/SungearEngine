//
// Created by stuka on 13.05.2023.
//

#include "Shader.h"

Core::Graphics::API::Shader::~Shader()
{
    m_defines.clear();
}

void Core::Graphics::API::Shader::addShaderDefine(Core::Graphics::API::ShaderDefine& shaderDefine)
{
    if(std::find(m_defines.begin(), m_defines.end(), shaderDefine) != m_defines.end()) return;

    m_defines.push_back(shaderDefine);
}

void Core::Graphics::API::Shader::removeShaderDefine(Core::Graphics::API::ShaderDefine& shaderDefine)
{
    m_defines.remove(shaderDefine);
}

void Core::Graphics::API::Shader::removeShaderDefine(const std::string& shaderDefineName)
{
    m_defines.remove(ShaderDefine(shaderDefineName, ""));
}