//
// Created by stuka on 13.05.2023.
//

#pragma once

#ifndef NATIVECORE_SHADERDEFINE_H
#define NATIVECORE_SHADERDEFINE_H

#include <string>

namespace Core::Graphics::API
{
    struct ShaderDefine
    {
        std::string m_name;
        std::string m_expression;

        ShaderDefine() = default;
        ShaderDefine(const std::string& name, const std::string& expression) : m_name(name), m_expression(expression) { }

        bool operator ==(const ShaderDefine& other) const noexcept;
    };
}

#endif //NATIVECORE_SHADERDEFINE_H
