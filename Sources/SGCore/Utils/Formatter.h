//
// Created by ilya on 27.06.24.
//

#pragma once

#include <string>
#include <any>
#include <unordered_map>
#include <sgcore_export.h>

namespace SGCore
{
    struct SGCORE_EXPORT Formatter
    {
        std::string m_keyOpenExpr = "${";
        std::string m_keyCloseExpr = "}$";
        
        std::string format(const std::string& text) const;
        
        void clearMarkup() noexcept;
        
        std::any& operator[](const std::string& key) noexcept;
        
    private:
        std::unordered_map<std::string, std::any> m_markup;
    };
}
