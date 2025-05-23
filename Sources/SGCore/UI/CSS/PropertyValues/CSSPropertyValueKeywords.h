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
    enum struct UniversalKeyword
    {
        CSS_DECLARE_UNIVERSAL_KEYWORDS
    };

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

    enum struct FontFamilyKeyword
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

    enum struct FontSizeKeyword
    {
        CSS_DECLARE_UNIVERSAL_KEYWORDS,

        KW_XX_SMALL,
        KW_X_SMALL,
        KW_SMALL,
        KW_MEDIUM,
        KW_LARGE,
        KW_X_LARGE,
        KW_XX_LARGE,
        KW_SMALLER,
        KW_LARGER
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
        if(value == "initial")
        {
            return KeywordEnumT::KW_INITIAL;
        }
        if(value == "unset")
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
        if(value == "none")
        {
            return PositionAndSizeKeyword::KW_NONE;
        }
        if(value == "normal")
        {
            return PositionAndSizeKeyword::KW_NORMAL;
        }
        if(value == "cover")
        {
            return PositionAndSizeKeyword::KW_COVER;
        }
        if(value == "contain")
        {
            return PositionAndSizeKeyword::KW_CONTAIN;
        }
        if(value == "fixed")
        {
            return PositionAndSizeKeyword::KW_FIXED;
        }
        if(value == "absolute")
        {
            return PositionAndSizeKeyword::KW_ABSOLUTE;
        }
        if(value == "relative")
        {
            return PositionAndSizeKeyword::KW_RELATIVE;
        }
        if(value == "static")
        {
            return PositionAndSizeKeyword::KW_STATIC;
        }
        if(value == "sticky")
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
        if(value == "currentColor")
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
        if(value == "inline")
        {
            return DisplayKeyword::KW_INLINE;
        }
        if(value == "flex")
        {
            return DisplayKeyword::KW_FLEX;
        }
        if(value == "grid")
        {
            return DisplayKeyword::KW_GRID;
        }
        if(value == "inline-block")
        {
            return DisplayKeyword::KW_INLINE_BLOCK;
        }
        if(value == "inline-flex")
        {
            return DisplayKeyword::KW_INLINE_FLEX;
        }
        if(value == "inline-grid")
        {
            return DisplayKeyword::KW_INLINE_GRID;
        }
        if(value == "table")
        {
            return DisplayKeyword::KW_TABLE;
        }
        if(value == "none")
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
        if(value == "end")
        {
            return FlexboxKeyword::KW_END;
        }
        if(value == "center")
        {
            return FlexboxKeyword::KW_CENTER;
        }
        if(value == "stretch")
        {
            return FlexboxKeyword::KW_STRETCH;
        }
        if(value == "space-between")
        {
            return FlexboxKeyword::KW_SPACE_BETWEEN;
        }
        if(value == "space-around")
        {
            return FlexboxKeyword::KW_SPACE_AROUND;
        }
        if(value == "space-evenly")
        {
            return FlexboxKeyword::KW_SPACE_EVENLY;
        }
        if(value == "row")
        {
            return FlexboxKeyword::KW_ROW;
        }
        if(value == "row-reverse")
        {
            return FlexboxKeyword::KW_ROW_REVERSE;
        }
        if(value == "column")
        {
            return FlexboxKeyword::KW_COLUMN;
        }
        if(value == "column-reverse")
        {
            return FlexboxKeyword::KW_COLUMN_REVERSE;
        }
        if(value == "wrap")
        {
            return FlexboxKeyword::KW_WRAP;
        }
        if(value == "nowrap")
        {
            return FlexboxKeyword::KW_NOWRAP;
        }
        if(value == "wrap-reverse")
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
        else if constexpr(std::is_same_v<KeywordEnumT, UniversalKeyword>)
        {
            return getUniversalKeywordFromStringValue<UniversalKeyword>(value);
        }
        else
        {
            static_assert(always_false<KeywordEnumT>::value, "Unknown keyword enum type.");
        }
    }
}

#endif //SUNGEARENGINE_CSSPROPERTYVALUEKEYWORDS_H
