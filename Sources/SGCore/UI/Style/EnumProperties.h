#pragma once

#include <string>

namespace SGCore::UI {
    #define define_property(enumPropName) SG_##enumPropName,
    #define define_enum(name, content) \
        enum class name { \
            content \
        };

    #include "EnumPropertiesIter.h"

    [[nodiscard]] constexpr char toLower(const char c) noexcept {
        return c >= 'A' && c <= 'Z' ? c + ('a' - 'A') : c;
    }
    [[nodiscard]] constexpr char toCapital(const char c) noexcept {
        return c >= 'a' && c <= 'z' ? c + ('A' - 'a') : c;
    }

    template<std::size_t N>
    struct ConstStringResult { char m_val[N]; consteval explicit ConstStringResult(char val[N]) {
        std::copy(val, val + N, m_val);
    } };

    template<std::size_t N>
    [[nodiscard]] consteval ConstStringResult<N> toKebabCase(const char (&old_str) [N]) noexcept {
        char str[N] = {'\0'};
        std::copy(old_str, old_str + N, str);

        for (size_t i = 0; i < N; ++i) {
            str[i] = toLower(str[i]);
            if (str[i] == '_') {
                str[i] = '-';
            }
        }

        return ConstStringResult<N>(str);
    }

    template<typename T>
    [[nodiscard]] std::optional<std::string_view> enumPropertyValueToCebabCaseName([[maybe_unused]] T property) noexcept {
        return std::nullopt;
    }
    /*#define define_property(property) \
        case EnumType::property: \
            return toKebabCase(#property).m_val;
    #define define_enum(enumName, content) \
        template<> \
        [[nodiscard]] std::optional<std::string_view> enumPropertyValueToCebabCaseName<enumName>(enumName property) noexcept { \
            using EnumType = enumName; \
            switch (property) { \
                content \
            } \
            return std::nullopt; \
        }

    #include "EnumPropertiesIter.h"*/

    template<std::size_t N>
    [[nodiscard]] consteval ConstStringResult<N> toCamelCase(const char (&source)[N]) noexcept {
        char str[N] = {'\0'};

        str[0] = toCapital(source[0]);
        for (size_t sourceI = 1, strI = 1; sourceI < N; sourceI++, strI++) {
            if (str[strI] == '_') {
                str[strI] = toCapital(source[++sourceI]);
            } else {
                str[strI] = toLower(source[sourceI]);
            }
        }

        return ConstStringResult<N>(str);
    }

    template<typename T>
    [[nodiscard]] inline std::optional<std::string_view> enumPropertyValueToCamelCase(T property) noexcept {
        return std::nullopt;
    }
    /*#define define_property(property) \
        case EnumType::property: \
            return toCamelCase(#property).m_val;
    #define define_enum(enumName, content) \
        template<> \
        [[nodiscard]] std::optional<std::string_view> enumPropertyValueToCamelCase<enumName>(enumName property) noexcept { \
            using EnumType = enumName; \
            switch (property) { \
                content \
            } \
        return std::nullopt; \
    }

    #include "EnumPropertiesIter.h"*/
}