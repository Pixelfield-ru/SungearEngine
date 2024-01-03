//
// Created by stuka on 28.12.2023.
//

#ifndef SGSLINTERPRETER_SGSLVARIABLE_H
#define SGSLINTERPRETER_SGSLVARIABLE_H

#include <string>

namespace SGCore
{
    struct SGSLVariable
    {
        std::string m_type;
        std::string m_name;
        // may be empty
        std::string m_rsideFunction;
        // may be empty
        std::vector<std::string> m_rsideFunctionArgs;
        bool m_isArray = false;
        size_t m_count = 0;

        std::strong_ordering operator<=>(const SGSLVariable& other) const noexcept
        {
            return m_name <=> other.m_name;
        }
    };
}

#endif //SGSLINTERPRETER_SGSLVARIABLE_H
