//
// Created by stuka on 19.01.2025.
//

#ifndef SUNGEARENGINE_CSSFLEXDIRECTIONTYPE_H
#define SUNGEARENGINE_CSSFLEXDIRECTIONTYPE_H

#include <string>

namespace SGCore::UI
{
    enum CSSFlexDirectionType
    {
        FDT_ROW,
        FDT_ROW_REVERSE,
        FDT_COLUMN,
        FDT_COLUMN_REVERSE,

        FDT_UNKNOWN
    };

    [[nodiscard]] static CSSFlexDirectionType getFlexDirectionTypeFromName(const std::string& propertyName) noexcept
    {
        if(propertyName == "row")
        {
            return CSSFlexDirectionType::FDT_ROW;
        }
        else if(propertyName == "row-reverse")
        {
            return CSSFlexDirectionType::FDT_ROW_REVERSE;
        }
        else if(propertyName == "column")
        {
            return CSSFlexDirectionType::FDT_COLUMN;
        }
        else if(propertyName == "column-reverse")
        {
            return CSSFlexDirectionType::FDT_COLUMN_REVERSE;
        }

        return CSSFlexDirectionType::FDT_UNKNOWN;
    }
}

#endif //SUNGEARENGINE_CSSFLEXDIRECTIONTYPE_H
