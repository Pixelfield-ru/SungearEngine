//
// Created by stuka on 26.02.2025.
//

#include "CSSPropertyType.h"

SGCore::UI::CSSPropertyType SGCore::UI::getPropertyTypeFromName(const std::string& propertyName) noexcept
{
    if(propertyName == "display")
    {
        return CSSPropertyType::PT_DISPLAY;
    }
    if(propertyName == "flex-direction")
    {
        return CSSPropertyType::PT_FLEX_DIRECTION;
    }
    if(propertyName == "flex-wrap")
    {
        return CSSPropertyType::PT_FLEX_WRAP;
    }
    if(propertyName == "justify-content")
    {
        return CSSPropertyType::PT_JUSTIFY_CONTENT;
    }
    if(propertyName == "align-items")
    {
        return CSSPropertyType::PT_ALIGN_ITEMS;
    }
    if(propertyName == "align-content")
    {
        return CSSPropertyType::PT_ALIGN_CONTENT;
    }
    if(propertyName == "gap")
    {
        return CSSPropertyType::PT_GAP;
    }
    if(propertyName == "order")
    {
        return CSSPropertyType::PT_ORDER;
    }
    if(propertyName == "flex-grow")
    {
        return CSSPropertyType::PT_FLEX_GROW;
    }
    if(propertyName == "flex-shrink")
    {
        return CSSPropertyType::PT_FLEX_SHRINK;
    }
    if(propertyName == "width")
    {
        return CSSPropertyType::PT_WIDTH;
    }
    if(propertyName == "height")
    {
        return CSSPropertyType::PT_HEIGHT;
    }
    if(propertyName == "background-color")
    {
        return CSSPropertyType::PT_BACKGROUND_COLOR;
    }
    if(propertyName == "border-radius")
    {
        return CSSPropertyType::PT_BORDER_RADIUS;
    }
    if(propertyName == "padding")
    {
        return CSSPropertyType::PT_PADDING;
    }

    return CSSPropertyType::PT_UNKNOWN;
}
