//
// Created by stuka on 13.05.2023.
//

#include "ShaderDefine.h"

bool SGCore::ShaderDefine::operator==(const ShaderDefine& other) const noexcept
{
    return this->m_name == other.m_name;
}

std::string SGCore::ShaderDefine::toString() const noexcept
{
    return "#define " + m_name + " " + m_expression;
}