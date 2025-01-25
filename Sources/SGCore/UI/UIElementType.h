//
// Created by stuka on 25.01.2025.
//

#ifndef SUNGEARENGINE_UIELEMENTTYPE_H
#define SUNGEARENGINE_UIELEMENTTYPE_H

#include <string>

namespace SGCore::UI
{
    enum struct UIElementType
    {
        ET_ROOT,
        ET_INCLUDE,
        ET_DIV,
        ET_TEXT,
        ET_UNKNOWN
    };

    static UIElementType getUIElementTypeFromString(const std::string& str) noexcept
    {
        if(str == "xml")
        {
            return UIElementType::ET_ROOT;
        }
        else if(str == "include")
        {
            return UIElementType::ET_INCLUDE;
        }
        else if(str == "div")
        {
            return UIElementType::ET_DIV;
        }
        else if(str == "text")
        {
            return UIElementType::ET_TEXT;
        }

        return UIElementType::ET_UNKNOWN;
    }
}

#endif //SUNGEARENGINE_UIELEMENTTYPE_H
