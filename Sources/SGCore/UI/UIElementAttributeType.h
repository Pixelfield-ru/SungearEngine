//
// Created by stuka on 25.01.2025.
//

#pragma once

#include <string>

namespace SGCore::UI
{
    enum struct UIElementAttributeType
    {
        AT_CLASS,   // for any element
        AT_NAME,    // for any element
        AT_TYPE,    // for 'include' element
        AT_PATH,    // for 'include' element
        AT_UNKNOWN
    };

    static UIElementAttributeType getUIElementAttributeTypeFromString(const std::string& value) noexcept
    {
        if(value == "class")
        {
            return UIElementAttributeType::AT_CLASS;
        }
        if(value == "type")
        {
            return UIElementAttributeType::AT_TYPE;
        }
        if(value == "path")
        {
            return UIElementAttributeType::AT_PATH;
        }
        if(value == "name")
        {
            return UIElementAttributeType::AT_NAME;
        }

        return UIElementAttributeType::AT_UNKNOWN;
    }
}
