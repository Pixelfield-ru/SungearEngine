//
// Created by stuka on 13.05.2023.
//

#pragma once

#ifndef NATIVECORE_SHADERDEFINE_H
#define NATIVECORE_SHADERDEFINE_H

#include <string>
#include <utility>

namespace Core::Graphics::API
{
    struct ShaderDefine
    {
        std::string m_name;
        std::string m_expression;

        ShaderDefine() = default;
        ShaderDefine(std::string name, std::string expression) : m_name(std::move(name)), m_expression(std::move(expression)) { }

        [[nodiscard]] std::string toString() const noexcept;

        bool operator ==(const ShaderDefine& other) const noexcept;
    };
}

#endif //NATIVECORE_SHADERDEFINE_H
