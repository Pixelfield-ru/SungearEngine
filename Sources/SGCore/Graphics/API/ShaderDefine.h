//
// Created by stuka on 13.05.2023.
//

#pragma once

#include <string>
#include <sgcore_export.h>

namespace SGCore
{
    struct SGCORE_EXPORT ShaderDefine
    {
        std::string m_name;
        std::string m_expression;

        ShaderDefine() = default;
        ShaderDefine(std::string name, std::string expression) : m_name(std::move(name)), m_expression(std::move(expression)) { }

        [[nodiscard]] std::string toString() const noexcept;

        bool operator ==(const ShaderDefine& other) const noexcept;
    };
}
