//
// Created by stuka on 22.01.2025.
//

#ifndef SUNGEARENGINE_CSSPROPERTYVALUEKEYWORDS_H
#define SUNGEARENGINE_CSSPROPERTYVALUEKEYWORDS_H

#include <cstdint>

#include "SGCore/UI/CSS/CSSPropertyType.h"

#include "SGCore/Utils/TypeTraits.h"

#define CSS_KEYWORD(name, value) static constexpr std::int64_t name = value;
#define CSS_DECLARE_UNIVERSAL_KEYWORDS KW_INHERIT, KW_INITIAL, KW_UNSET, KW_UNKNOWN

namespace SGCore::UI
{
    enum struct PositionAndSizeKeyword
    {
        CSS_DECLARE_UNIVERSAL_KEYWORDS,

        KW_AUTO,
        KW_NONE,
        KW_NORMAL,
        KW_COVER,
        KW_CONTAIN,
        KW_FIXED,
        KW_ABSOLUTE,
        KW_RELATIVE,
        KW_STATIC,
        KW_STICKY,
    };

    enum struct ColorKeyword
    {
        CSS_DECLARE_UNIVERSAL_KEYWORDS,

        KW_TRANSPARENT,
        KW_CURRENT_COLOR
    };

    enum struct FontKeyword
    {
        CSS_DECLARE_UNIVERSAL_KEYWORDS,

        KW_SERIF,
        KW_SANS_SERIF,
        KW_MONOSPACE,
        KW_CURSIVE,
        KW_FANTASY,
        KW_BOLD,
        KW_NORMAL,
        KW_BOLDER,
        KW_LIGHTER,
        KW_ITALIC,
        KW_OBLIQUE
    };

    enum struct DisplayKeyword // prop: display
    {
        CSS_DECLARE_UNIVERSAL_KEYWORDS,

        KW_BLOCK,
        KW_INLINE,
        KW_FLEX,
        KW_GRID,
        KW_INLINE_BLOCK,
        KW_INLINE_FLEX,
        KW_INLINE_GRID,
        KW_TABLE,
        KW_NONE
    };

    enum struct ContentOverflowKeyword
    {
        CSS_DECLARE_UNIVERSAL_KEYWORDS,

        KW_CLIP,
        KW_ELLIPSIS,
        KW_VISIBLE,
        KW_HIDDEN,
        KW_SCROLL,
        KW_AUTO
    };

    enum struct AnimationKeyword
    {
        CSS_DECLARE_UNIVERSAL_KEYWORDS,

        KW_EASE,
        KW_LINEAR,
        KW_EASE_IN,
        KW_EASE_OUT,
        KW_EASE_IN_OUT,
        KW_INFINITE,
        KW_FORWARDS,
        KW_BACKWARDS
    };

    enum struct ListKeyword
    {
        CSS_DECLARE_UNIVERSAL_KEYWORDS,

        KW_DISC,
        KW_CIRCLE,
        KW_SQUARE,
        KW_DECIMAL,
        KW_NONE
    };

    enum struct BorderKeyword
    {
        CSS_DECLARE_UNIVERSAL_KEYWORDS,

        KW_SOLID,
        KW_DASHED,
        KW_DOTTED,
        KW_DOUBLE,
        KW_GROOVE,
        KW_RIDGE,
        KW_INSET,
        KW_OUTSET,
        KW_NONE,
        KW_HIDDEN
    };

    enum struct FlexboxKeyword
    {
        CSS_DECLARE_UNIVERSAL_KEYWORDS,

        KW_START,                   // prop: justify-content, align-items
        KW_END,                     // prop: justify-content, align-items
        KW_CENTER,                  // prop: justify-content, align-items
        KW_STRETCH,                 // prop: justify-content, align-items
        KW_SPACE_BETWEEN,           // prop: justify-content, align-items
        KW_SPACE_AROUND,            // prop: justify-content, align-items
        KW_SPACE_EVENLY,            // prop: justify-content, align-items
        KW_ROW,                     // prop: flex-direction
        KW_ROW_REVERSE,             // prop: flex-direction
        KW_COLUMN,                  // prop: flex-direction
        KW_COLUMN_REVERSE,          // prop: flex-direction
        KW_WRAP,                    // prop: flex-wrap
        KW_NOWRAP,                  // prop: flex-wrap
        KW_WRAP_REVERSE             // prop: flex-wrap
    };

    enum struct ShadowKeyword
    {
        CSS_DECLARE_UNIVERSAL_KEYWORDS,

        KW_NONE,
        KW_INSET
    };

    template<typename KeywordEnumT>
    [[nodiscard]] static KeywordEnumT getUniversalKeywordFromStringValue(const std::string& value) noexcept
    {
        if(value == "inherit")
        {
            return KeywordEnumT::KW_INHERIT;
        }
        else if(value == "initial")
        {
            return KeywordEnumT::KW_INITIAL;
        }
        else if(value == "unset")
        {
            return KeywordEnumT::KW_UNSET;
        }

        return KeywordEnumT::KW_UNKNOWN;
    }

    [[nodiscard]] static PositionAndSizeKeyword getPositionAndSizeKeywordFromStringValue(const std::string& value) noexcept
    {
        if(value == "auto")
        {
            return PositionAndSizeKeyword::KW_AUTO;
        }
        else if(value == "none")
        {
            return PositionAndSizeKeyword::KW_NONE;
        }
        else if(value == "normal")
        {
            return PositionAndSizeKeyword::KW_NORMAL;
        }
        else if(value == "cover")
        {
            return PositionAndSizeKeyword::KW_COVER;
        }
        else if(value == "contain")
        {
            return PositionAndSizeKeyword::KW_CONTAIN;
        }
        else if(value == "fixed")
        {
            return PositionAndSizeKeyword::KW_FIXED;
        }
        else if(value == "absolute")
        {
            return PositionAndSizeKeyword::KW_ABSOLUTE;
        }
        else if(value == "relative")
        {
            return PositionAndSizeKeyword::KW_RELATIVE;
        }
        else if(value == "static")
        {
            return PositionAndSizeKeyword::KW_STATIC;
        }
        else if(value == "sticky")
        {
            return PositionAndSizeKeyword::KW_STICKY;
        }

        return getUniversalKeywordFromStringValue<PositionAndSizeKeyword>(value);
    }

    [[nodiscard]] static ColorKeyword getColorKeywordFromStringValue(const std::string& value) noexcept
    {
        if(value == "transparent")
        {
            return ColorKeyword::KW_TRANSPARENT;
        }
        else if(value == "currentColor")
        {
            return ColorKeyword::KW_CURRENT_COLOR;
        }

        return getUniversalKeywordFromStringValue<ColorKeyword>(value);
    }

    [[nodiscard]] static DisplayKeyword getDisplayKeywordFromStringValue(const std::string& value) noexcept
    {
        if(value == "block")
        {
            return DisplayKeyword::KW_BLOCK;
        }
        else if(value == "inline")
        {
            return DisplayKeyword::KW_INLINE;
        }
        else if(value == "flex")
        {
            return DisplayKeyword::KW_FLEX;
        }
        else if(value == "grid")
        {
            return DisplayKeyword::KW_GRID;
        }
        else if(value == "inline-block")
        {
            return DisplayKeyword::KW_INLINE_BLOCK;
        }
        else if(value == "inline-flex")
        {
            return DisplayKeyword::KW_INLINE_FLEX;
        }
        else if(value == "inline-grid")
        {
            return DisplayKeyword::KW_INLINE_GRID;
        }
        else if(value == "table")
        {
            return DisplayKeyword::KW_TABLE;
        }
        else if(value == "none")
        {
            return DisplayKeyword::KW_NONE;
        }

        return getUniversalKeywordFromStringValue<DisplayKeyword>(value);
    }

    [[nodiscard]] static FlexboxKeyword getFlexboxKeywordFromStringValue(const std::string& value) noexcept
    {
        if(value == "start")
        {
            return FlexboxKeyword::KW_START;
        }
        else if(value == "end")
        {
            return FlexboxKeyword::KW_END;
        }
        else if(value == "center")
        {
            return FlexboxKeyword::KW_CENTER;
        }
        else if(value == "stretch")
        {
            return FlexboxKeyword::KW_STRETCH;
        }
        else if(value == "space-between")
        {
            return FlexboxKeyword::KW_SPACE_BETWEEN;
        }
        else if(value == "space-around")
        {
            return FlexboxKeyword::KW_SPACE_AROUND;
        }
        else if(value == "space-evenly")
        {
            return FlexboxKeyword::KW_SPACE_EVENLY;
        }
        else if(value == "row")
        {
            return FlexboxKeyword::KW_ROW;
        }
        else if(value == "row-reverse")
        {
            return FlexboxKeyword::KW_ROW_REVERSE;
        }
        else if(value == "column")
        {
            return FlexboxKeyword::KW_COLUMN;
        }
        else if(value == "column-reverse")
        {
            return FlexboxKeyword::KW_COLUMN_REVERSE;
        }
        else if(value == "wrap")
        {
            return FlexboxKeyword::KW_WRAP;
        }
        else if(value == "nowrap")
        {
            return FlexboxKeyword::KW_NOWRAP;
        }
        else if(value == "wrap-reverse")
        {
            return FlexboxKeyword::KW_WRAP_REVERSE;
        }

        return getUniversalKeywordFromStringValue<FlexboxKeyword>(value);
    }

    template<typename KeywordEnumT>
    requires(std::is_enum_v<KeywordEnumT>)
    static KeywordEnumT getKeywordFromStringValue(const std::string& value) noexcept
    {
        if constexpr(std::is_same_v<KeywordEnumT, PositionAndSizeKeyword>)
        {
            return getPositionAndSizeKeywordFromStringValue(value);
        }
        else if constexpr(std::is_same_v<KeywordEnumT, FlexboxKeyword>)
        {
            return getFlexboxKeywordFromStringValue(value);
        }
        else
        {
            static_assert(always_false<KeywordEnumT>::value, "Unknown keyword enum type.");
        }
    }
}

#endif //SUNGEARENGINE_CSSPROPERTYVALUEKEYWORDS_H
