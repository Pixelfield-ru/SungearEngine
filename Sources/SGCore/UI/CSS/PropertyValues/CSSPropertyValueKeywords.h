//
// Created by stuka on 22.01.2025.
//

#ifndef SUNGEARENGINE_CSSPROPERTYVALUEKEYWORDS_H
#define SUNGEARENGINE_CSSPROPERTYVALUEKEYWORDS_H

#include <cstdint>

#define CSS_KEYWORD(name, value) static constexpr std::int64_t name = value;

namespace SGCore::UI
{
    struct UniversalKeyword
    {
        CSS_KEYWORD(KW_INHERIT, 0)
        CSS_KEYWORD(KW_INITIAL, 1)
        CSS_KEYWORD(KW_UNSET, 2)
    };

    struct PositionAndSizeKeyword : UniversalKeyword
    {
        CSS_KEYWORD(KW_AUTO, 3)
        CSS_KEYWORD(KW_NONE, 4)
        CSS_KEYWORD(KW_NORMAL, 5)
        CSS_KEYWORD(KW_COVER, 6)
        CSS_KEYWORD(KW_CONTAIN, 7)
        CSS_KEYWORD(KW_FIXED, 8)
        CSS_KEYWORD(KW_ABSOLUTE, 9)
        CSS_KEYWORD(KW_RELATIVE, 10)
        CSS_KEYWORD(KW_STATIC, 11)
        CSS_KEYWORD(KW_STICKY, 12)
    };

    struct ColorKeyword : UniversalKeyword
    {
        CSS_KEYWORD(KW_TRANSPARENT, 3)
        CSS_KEYWORD(KW_CURRENT_COLOR, 4)
    };

    struct FontKeyword : UniversalKeyword
    {
        CSS_KEYWORD(KW_SERIF, 3)
        CSS_KEYWORD(KW_SANS_SERIF, 4)
        CSS_KEYWORD(KW_MONOSPACE, 5)
        CSS_KEYWORD(KW_CURSIVE, 6)
        CSS_KEYWORD(KW_FANTASY, 7)
        CSS_KEYWORD(KW_BOLD, 8)
        CSS_KEYWORD(KW_NORMAL, 9)
        CSS_KEYWORD(KW_BOLDER, 10)
        CSS_KEYWORD(KW_LIGHTER, 11)
        CSS_KEYWORD(KW_ITALIC, 12)
        CSS_KEYWORD(KW_OBLIQUE, 13)
    };

    struct DisplayKeyword : UniversalKeyword
    {
        CSS_KEYWORD(KW_BLOCK, 3)
        CSS_KEYWORD(KW_INLINE, 4)
        CSS_KEYWORD(KW_FLEX, 5)
        CSS_KEYWORD(KW_GRID, 6)
        CSS_KEYWORD(KW_INLINE_BLOCK, 7)
        CSS_KEYWORD(KW_INLINE_FLEX, 8)
        CSS_KEYWORD(KW_INLINE_GRID, 9)
        CSS_KEYWORD(KW_TABLE, 10)
        CSS_KEYWORD(KW_NONE, 11)
    };

    struct ContentOverflowKeyword : UniversalKeyword
    {
        CSS_KEYWORD(KW_CLIP, 3)
        CSS_KEYWORD(KW_ELLIPSIS, 4)
        CSS_KEYWORD(KW_VISIBLE, 5)
        CSS_KEYWORD(KW_HIDDEN, 6)
        CSS_KEYWORD(KW_SCROLL, 7)
        CSS_KEYWORD(KW_AUTO, 8)
    };

    struct AnimationKeyword : UniversalKeyword
    {
        CSS_KEYWORD(KW_EASE, 3)
        CSS_KEYWORD(KW_LINEAR, 4)
        CSS_KEYWORD(KW_EASE_IN, 5)
        CSS_KEYWORD(KW_EASE_OUT, 6)
        CSS_KEYWORD(KW_EASE_IN_OUT, 7)
        CSS_KEYWORD(KW_INFINITE, 8)
        CSS_KEYWORD(KW_FORWARDS, 9)
        CSS_KEYWORD(KW_BACKWARDS, 10)
    };

    struct ListKeyword : UniversalKeyword
    {
        CSS_KEYWORD(KW_DISC, 3)
        CSS_KEYWORD(KW_CIRCLE, 4)
        CSS_KEYWORD(KW_SQUARE, 5)
        CSS_KEYWORD(KW_DECIMAL, 6)
        CSS_KEYWORD(KW_NONE, 7)
    };

    struct BorderKeyword : UniversalKeyword
    {
        CSS_KEYWORD(KW_SOLID, 3)
        CSS_KEYWORD(KW_DASHED, 4)
        CSS_KEYWORD(KW_DOTTED, 5)
        CSS_KEYWORD(KW_DOUBLE, 6)
        CSS_KEYWORD(KW_GROOVE, 7)
        CSS_KEYWORD(KW_RIDGE, 8)
        CSS_KEYWORD(KW_INSET, 9)
        CSS_KEYWORD(KW_OUTSET, 11)
        CSS_KEYWORD(KW_NONE, 12)
        CSS_KEYWORD(KW_HIDDEN, 13)
    };

    struct FlexboxKeyword : UniversalKeyword
    {
        CSS_KEYWORD(KW_START, 3)
        CSS_KEYWORD(KW_END, 4)
        CSS_KEYWORD(KW_CENTER, 5)
        CSS_KEYWORD(KW_STRETCH, 6)
        CSS_KEYWORD(KW_SPACE_BETWEEN, 7)
        CSS_KEYWORD(KW_SPACE_AROUND, 8)
        CSS_KEYWORD(KW_SPACE_EVENLY, 9)
    };

    struct ShadowKeyword : UniversalKeyword
    {
        CSS_KEYWORD(KW_NONE, 3)
        CSS_KEYWORD(KW_INSET, 4)
    };
}

#endif //SUNGEARENGINE_CSSPROPERTYVALUEKEYWORDS_H
