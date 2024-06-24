//
// Created by ilya on 11.02.24.
//

#ifndef SGSLETRANSLATOR_SGSLESTRUCT_H
#define SGSLETRANSLATOR_SGSLESTRUCT_H

#include <SGCore/pch.h>

namespace SGCore
{
    struct SGSLEStruct
    {
        std::string m_name;
        std::vector<std::string> m_bases;
        std::vector<std::string> m_variables;
        
        bool operator==(const SGSLEStruct& other) const noexcept
        {
            return m_name == other.m_name;
        }
        
        bool operator!=(const SGSLEStruct& other) const noexcept
        {
            return !(*this == other);
        }
        
        [[nodiscard]] bool hasBase(const std::string& baseStructName) const noexcept
        {
            return std::find(m_bases.begin(), m_bases.end(), baseStructName) != m_bases.end();
        }
    };
}

#endif //SGSLETRANSLATOR_SGSLESTRUCT_H
