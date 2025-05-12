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
        PT_DISPLAY,
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
        PT_BORDER_RADIUS,
        PT_PADDING,
        PT_FONT_FAMILY,
        PT_FONT_SIZE,

        PT_UNKNOWN,
    };

    [[nodiscard]] CSSPropertyType getPropertyTypeFromName(const std::string& propertyName) noexcept;

    /*static std::string getDefaultPropertyValueString(CSSPropertyType propertyType) noexcept
    {
        switch(propertyType)
        {
            case CSSPropertyType::PT_DISPLAY: return "flex";
            case CSSPropertyType::PT_FLEX_DIRECTION: return "row";
            case CSSPropertyType::PT_FLEX_WRAP: return "wrap";
            case CSSPropertyType::PT_JUSTIFY_CONTENT: return "start";
            case CSSPropertyType::PT_ALIGN_ITEMS: return "start";
            case CSSPropertyType::PT_ALIGN_CONTENT: return "stretch";
            case CSSPropertyType::PT_GAP: return "0";
            case CSSPropertyType::PT_ORDER: return "0";
            case CSSPropertyType::PT_FLEX_GROW: return "0";
            case CSSPropertyType::PT_FLEX_SHRINK: return "1";
            case CSSPropertyType::PT_WIDTH: return "auto";
            case CSSPropertyType::PT_HEIGHT: return "height";
            case CSSPropertyType::PT_BACKGROUND_COLOR: return "transparent";
            case CSSPropertyType::PT_UNKNOWN:break;
        }

        return "inherit";
    }*/
}

#endif //SUNGEARENGINE_CSSPROPERTYTYPE_H
