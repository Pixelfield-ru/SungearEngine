#pragma once

#include "SGCore/Utils/Macroses.h"

#define UNIVERSAL_KEYWORDS(defineProperty) defineProperty(INHERIT) defineProperty(INITIAL) defineProperty(UNSET) defineProperty(UNKNOWN)

// iterate through all enum properties
#define enum_properties(defineEnum, defineProperty) \
defineEnum(UniversalKeyword, UNIVERSAL_KEYWORDS(defineProperty)) \
defineEnum( \
    PositionAndSizeKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(AUTO) \
        defineProperty(NONE) \
        defineProperty(NORMAL) \
        defineProperty(COVER) \
        defineProperty(CONTAIN) \
        defineProperty(FIXED) \
        defineProperty(ABSOLUTE) \
        defineProperty(RELATIVE) \
        defineProperty(STATIC) \
        defineProperty(STICKY) \
) \
defineEnum( \
    ColorKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(TRANSPARENT) \
        defineProperty(CURRENT_COLOR) \
) \
defineEnum( \
    FontFamilyKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(SERIF) \
        defineProperty(SANS_SERIF) \
        defineProperty(MONOSPACE) \
        defineProperty(CURSIVE) \
        defineProperty(FANTASY) \
        defineProperty(BOLD) \
        defineProperty(NORMAL) \
        defineProperty(BOLDER) \
        defineProperty(LIGHTER) \
        defineProperty(ITALIC) \
        defineProperty(OBLIQUE) \
) \
defineEnum( \
    FontSizeKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(XX_SMALL) \
        defineProperty(X_SMALL) \
        defineProperty(SMALL) \
        defineProperty(MEDIUM) \
        defineProperty(LARGE) \
        defineProperty(X_LARGE) \
        defineProperty(XX_LARGE) \
        defineProperty(SMALLER) \
        defineProperty(LARGER) \
) \
defineEnum( \
    DisplayKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(BLOCK) \
        defineProperty(INLINE) \
        defineProperty(FLEX) \
        defineProperty(GRID) \
        defineProperty(INLINE_BLOCK) \
        defineProperty(INLINE_FLEX) \
        defineProperty(INLINE_GRID) \
        defineProperty(TABLE) \
        defineProperty(NONE) \
) \
defineEnum( \
    ContentOverflowKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(CLIP) \
        defineProperty(ELLIPSIS) \
        defineProperty(VISIBLE) \
        defineProperty(HIDDEN) \
        defineProperty(SCROLL) \
        defineProperty(AUTO) \
) \
defineEnum( \
    AnimationKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(EASE) \
        defineProperty(LINEAR) \
        defineProperty(EASE_IN) \
        defineProperty(EASE_OUT) \
        defineProperty(EASE_IN_OUT) \
        defineProperty(INFINITE) \
        defineProperty(FORWARDS) \
        defineProperty(BACKWARDS) \
) \
defineEnum( \
    ListKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(DISC) \
        defineProperty(CIRCLE) \
        defineProperty(SQUARE) \
        defineProperty(DECIMAL) \
        defineProperty(NONE) \
) \
defineEnum( \
    BorderKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(SOLID) \
        defineProperty(DASHED) \
        defineProperty(DOTTED) \
        defineProperty(DOUBLE) \
        defineProperty(GROOVE) \
        defineProperty(RIDGE) \
        defineProperty(INSET) \
        defineProperty(OUTSET) \
        defineProperty(NONE) \
        defineProperty(HIDDEN) \
) \
defineEnum( \
    ShadowKeyword, \
        UNIVERSAL_KEYWORDS(defineProperty) \
        defineProperty(NONE) \
        defineProperty(INSET) \
) \
defineEnum( \
    FlexboxKeyword, \
        defineProperty(START) \
        defineProperty(END) \
        defineProperty(CENTER) \
        defineProperty(STRETCH) \
        defineProperty(SPACE_BETWEEN) \
        defineProperty(SPACE_AROUND) \
        defineProperty(SPACE_EVENLY) \
        defineProperty(ROW) \
        defineProperty(ROW_REVERSE) \
        defineProperty(COLUMN) \
        defineProperty(COLUMN_REVERSE) \
        defineProperty(WRAP) \
        defineProperty(NOWRAP) \
        defineProperty(WRAP_REVERSE) \
)


namespace SGCore::UI {
    #define define_prop(enumPropName) enumPropName,
    #define define_enum(name, content) \
        enum class name { \
            content \
        };

    enum_properties(define_enum, define_prop)

    #undef define_prop
    #undef define_enum

    [[nodiscard]] constexpr char toLower(const char c) noexcept {
        return c >= 'A' && c <= 'Z' ? c + ('a' - 'A') : c;
    }
    [[nodiscard]] constexpr char toCapital(const char c) noexcept {
        return c >= 'a' && c <= 'z' ? c + ('A' - 'a') : c;
    }

    [[nodiscard]] consteval std::string toKebabCase(std::string str) noexcept {
        for (size_t i = 0; i < str.length(); ++i) {
            str[i] = toLower(str[i]);
            if (str[i] == '_') {
                str[i] = '-';
            }
        }
        return str;
    }

    template<typename T>
    [[nodiscard]] std::optional<std::string> enumPropertyValueToCebabCaseName(T property) noexcept {
        return std::nullopt;
    }
    #define define_prop(property) \
        case EnumType::property: \
            return toKebabCase(#property);
    #define define_enum(enumName, content) \
        template<> \
        [[nodiscard]] std::optional<std::string> enumPropertyValueToCebabCaseName<enumName>(enumName property) noexcept { \
            using EnumType = enumName; \
            switch (property) { \
                content \
            } \
            return std::nullopt; \
        }

    enum_properties(define_enum, define_prop)

    #undef define_prop
    #undef define_enum

    [[nodiscard]] consteval std::string toCamelCase(std::string str) noexcept {
        str[0] = toCapital(str[0]);
        for (size_t i = 1; i < str.length(); ++i) {
            if (str[i] == '_') {
                str = str.substr(0, i) + str.substr(i + 1);
                --i;
            }
        }
        return str;
    }

    template<typename T>
    [[nodiscard]] inline std::optional<std::string> enumPropertyValueToCamelCase(T property) noexcept {
        return std::nullopt;
    }
    #define define_prop(property) \
        case EnumType::property: \
            return toCamelCase(#property);
    #define define_enum(enumName, content) \
        template<> \
        [[nodiscard]] std::optional<std::string> enumPropertyValueToCamelCase<enumName>(enumName property) noexcept { \
            using EnumType = enumName; \
            switch (property) { \
                content \
            } \
        return std::nullopt; \
    }

    enum_properties(define_enum, define_prop)

    #undef define_prop
    #undef define_enum
}

#undef UNIVERSAL_KEYWORDS
#undef enum_properties