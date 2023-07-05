//
// Created by stuka on 13.05.2023.
//

#include "ShaderDefine.h"

bool Core::Graphics::ShaderDefine::operator==(const Core::Graphics::ShaderDefine& other) const noexcept
{
    return this->m_name == other.m_name;
}

std::string Core::Graphics::ShaderDefine::toString() const noexcept
{
    return "#define " + m_name + " " + m_expression;
}