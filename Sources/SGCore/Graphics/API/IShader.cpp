//
// Created by stuka on 13.05.2023.
//

#include "IShader.h"

Core::Graphics::API::IShader::~IShader()
{
    m_defines.clear();
}

void Core::Graphics::API::IShader::addShaderDefine(Core::Graphics::API::ShaderDefine& shaderDefine)
{
    if(std::find(m_defines.begin(), m_defines.end(), shaderDefine) != m_defines.end()) return;

    m_defines.push_back(shaderDefine);
}

void Core::Graphics::API::IShader::removeShaderDefine(Core::Graphics::API::ShaderDefine& shaderDefine)
{
    m_defines.remove(shaderDefine);
}

void Core::Graphics::API::IShader::removeShaderDefine(const std::string& shaderDefineName)
{
    m_defines.remove(ShaderDefine(shaderDefineName, ""));
}