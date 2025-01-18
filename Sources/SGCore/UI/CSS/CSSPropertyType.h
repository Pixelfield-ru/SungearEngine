//
// Created by stuka on 17.01.2025.
//

#ifndef SUNGEARENGINE_CSSPROPERTYTYPE_H
#define SUNGEARENGINE_CSSPROPERTYTYPE_H

#include <string>

namespace SGCore::UI
{
    enum class CSSPropertyType
    {
        PT_FLEX_DIRECTION,
        PT_FLEX_WRAP,
        PT_JUSTIFY_CONTENT,
        PT_ALIGN_ITEMS,
        PT_ALIGN_CONTENT,
        PT_GAP,
        PT_ORDER,
        PT_FLEX_GROW,
        PT_FLEX_SHRINK,
        PT_WIDTH,
        PT_HEIGHT,
        PT_BACKGROUND_COLOR,

        PT_UNKNOWN
    };

    [[nodiscard]] static CSSPropertyType getPropertyTypeFromName(const std::string& propertyName) noexcept
    {
        if(propertyName == "flex-direction")
        {
            return CSSPropertyType::PT_FLEX_DIRECTION;
        }
        else if(propertyName == "flex-wrap")
        {
            return CSSPropertyType::PT_FLEX_WRAP;
        }
        else if(propertyName == "justify-content")
        {
            return CSSPropertyType::PT_JUSTIFY_CONTENT;
        }
        else if(propertyName == "align-items")
        {
            return CSSPropertyType::PT_ALIGN_ITEMS;
        }
        else if(propertyName == "align-content")
        {
            return CSSPropertyType::PT_ALIGN_CONTENT;
        }
        else if(propertyName == "gap")
        {
            return CSSPropertyType::PT_GAP;
        }
        else if(propertyName == "order")
        {
            return CSSPropertyType::PT_ORDER;
        }
        else if(propertyName == "flex-grow")
        {
            return CSSPropertyType::PT_FLEX_GROW;
        }
        else if(propertyName == "flex-shrink")
        {
            return CSSPropertyType::PT_FLEX_SHRINK;
        }
        else if(propertyName == "width")
        {
            return CSSPropertyType::PT_WIDTH;
        }
        else if(propertyName == "height")
        {
            return CSSPropertyType::PT_HEIGHT;
        }
        else if(propertyName == "background-color")
        {
            return CSSPropertyType::PT_BACKGROUND_COLOR;
        }

        return CSSPropertyType::PT_UNKNOWN;
    }
}

#endif //SUNGEARENGINE_CSSPROPERTYTYPE_H
