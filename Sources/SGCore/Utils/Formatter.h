//
// Created by ilya on 27.06.24.
//

#ifndef SUNGEARENGINE_FORMATTER_H
#define SUNGEARENGINE_FORMATTER_H

#include <string>
#include <any>
#include <unordered_map>

namespace SGCore
{
    struct Formatter
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

#endif //SUNGEARENGINE_FORMATTER_H
