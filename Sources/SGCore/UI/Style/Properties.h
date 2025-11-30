#pragma once

#include "SGCore/Utils/Macroses.h"

#define UNIVERSAL_KEYWORDS(defineProperty) defineProperty(KW_INHERIT, inherit, inherit) defineProperty(KW_INITIAL, initial, initial) defineProperty(KW_UNSET, unset, unset) defineProperty(KW_UNKNOWN, unknown, unknown)

// iterate through all enum properties
#define enum_properties(defineEnum, defineProperty) \
defineEnum(UniversalKeyword, UNIVERSAL_KEYWORDS(defineProperty)) \
defineEnum( \
    PositionAndSizeKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(AUTO, auto, auto) \
        defineProperty(NONE, none, none) \
        defineProperty(NORMAL, normal, normal) \
        defineProperty(COVER, cover, cover) \
        defineProperty(CONTAIN, contain, contain) \
        defineProperty(FIXED, fixed, fixed) \
        defineProperty(ABSOLUTE, absolute, absolute) \
        defineProperty(RELATIVE, relative, relative) \
        defineProperty(STATIC, static, static) \
        defineProperty(STICKY, sticky, sticky) \
) \
defineEnum( \
    ColorKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(TRANSPARENT, transparent, transparent) \
        defineProperty(CURRENT_COLOR, current-color, currentColor) \
) \
defineEnum( \
    FontFamilyKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(SERIF, serif, serif) \
        defineProperty(SANS_SERIF, sans-serif, sansSerif) \
        defineProperty(MONOSPACE, monospace, monospace) \
        defineProperty(CURSIVE, cursive, cursive) \
        defineProperty(FANTASY, fantasy, fantasy) \
        defineProperty(BOLD, bold, bold) \
        defineProperty(NORMAL, normal, normal) \
        defineProperty(BOLDER, bolder, bolder) \
        defineProperty(LIGHTER, lighter, lighter) \
        defineProperty(ITALIC, italic, italic) \
        defineProperty(OBLIQUE, oblique, oblique) \
) \
defineEnum( \
    FontSizeKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(XX_SMALL, xx-small, xxSmall) \
        defineProperty(X_SMALL, x-small, xSmall) \
        defineProperty(SMALL, small, small) \
        defineProperty(MEDIUM, medium, medium) \
        defineProperty(LARGE, large, large) \
        defineProperty(X_LARGE, x-large, xLarge) \
        defineProperty(XX_LARGE, xx-large, xxLarge) \
        defineProperty(SMALLER, smaller, smaller) \
        defineProperty(LARGER, larger, larger) \
) \
defineEnum( \
    DisplayKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(BLOCK, block, block) \
        defineProperty(INLINE, inline, inline) \
        defineProperty(FLEX, flex, flex) \
        defineProperty(GRID, grid, grid) \
        defineProperty(INLINE_BLOCK, inline-block, inlineBlock) \
        defineProperty(INLINE_FLEX, inline-flex, inlineFlex) \
        defineProperty(INLINE_GRID, inline-grid, inlineGrid) \
        defineProperty(TABLE, table, table) \
        defineProperty(NONE, none, none) \
) \
defineEnum( \
    ContentOverflowKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(CLIP, clip, clip) \
        defineProperty(ELLIPSIS, ellipsis, ellipsis) \
        defineProperty(VISIBLE, visible, visible) \
        defineProperty(HIDDEN, hidden, hidden) \
        defineProperty(SCROLL, scroll, scroll) \
        defineProperty(AUTO, auto, auto) \
) \
defineEnum( \
    AnimationKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(EASE, ease, ease) \
        defineProperty(LINEAR, linear, linear) \
        defineProperty(EASE_IN, ease-in, easeIn) \
        defineProperty(EASE_OUT, ease-out, easeOut) \
        defineProperty(EASE_IN_OUT, ease-in-out, easeInOut) \
        defineProperty(INFINITE, infinite, infinite) \
        defineProperty(FORWARDS, forwards, forwards) \
        defineProperty(BACKWARDS, backwards, backwards) \
) \
defineEnum( \
    ListKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(DISC, disc, disc) \
        defineProperty(CIRCLE, circle, circle) \
        defineProperty(SQUARE, square, square) \
        defineProperty(DECIMAL, decimal, decimal) \
        defineProperty(NONE, none, none) \
) \
defineEnum( \
    BorderKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(SOLID, solid, solid) \
        defineProperty(DASHED, dashed, dashed) \
        defineProperty(DOTTED, dotted, dotted) \
        defineProperty(DOUBLE, double, double) \
        defineProperty(GROOVE, groove, groove) \
        defineProperty(RIDGE, ridge, ridge) \
        defineProperty(INSET, inset, inset) \
        defineProperty(OUTSET, outset, outset) \
        defineProperty(NONE, none, none) \
        defineProperty(HIDDEN, hidden, hidden) \
) \
defineEnum( \
    ShadowKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(NONE, none, none) \
        defineProperty(INSET, inset, inset) \
) \
defineEnum( \
    FlexboxKeyword, \
        defineProperty(START, start, start) \
        defineProperty(END, end, end) \
        defineProperty(CENTER, center, center) \
        defineProperty(STRETCH, stretch, stretch) \
        defineProperty(SPACE_BETWEEN, space-between, spaceBetween) \
        defineProperty(SPACE_AROUND, space-around, spaceAround) \
        defineProperty(SPACE_EVENLY, space-evenly, spaceEvenly) \
        defineProperty(ROW, row, row) \
        defineProperty(ROW_REVERSE, row-reverse, rowReverse) \
        defineProperty(COLUMN, column, column) \
        defineProperty(COLUMN_REVERSE, column-reverse, columnReverse) \
        defineProperty(WRAP, wrap, wrap) \
        defineProperty(NOWRAP, nowrap, nowrap) \
        defineProperty(WRAP_REVERSE, wrap-reverse, wrapReverse) \
)


namespace SGCore::UI {
    #define defineProp(enumName, cssName, camelCaseName) enumName,
    #define defineEnum(name, content) \
        enum name { \
            content \
        };

    enum_properties(defineEnum, defineProp)

    #undef defineProp
    #undef defineEnum
}