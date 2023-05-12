//
// Created by stuka on 13.05.2023.
//

#include "ShaderDefine.h"

bool Core::Graphics::API::ShaderDefine::operator==(const Core::Graphics::API::ShaderDefine& other) const noexcept
{
    return this->m_name == other.m_name;
}