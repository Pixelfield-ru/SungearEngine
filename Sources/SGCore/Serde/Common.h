//
// Created by stuka on 20.10.2024.
//

#ifndef SUNGEARENGINE_SERDE_COMMON_H
#define SUNGEARENGINE_SERDE_COMMON_H

#include "SGCore/Utils/TypeTraits.h"

namespace SGCore::Serde
{
    template<typename... Types>
    using custom_derived_types = types_container<Types...>;

    template<typename T>
    concept custom_derived_types_t = types_container_t<T>;

    enum class FormatType
    {
        JSON,
        BSON,
        YAML
    };

    template<typename T, FormatType TFormatType>
    struct SerdeSpec;

    namespace Detail
    {
        static inline constexpr char s_typeNameFieldName[] = "__sg_type_name";
        static inline constexpr char s_arrayTypeNameFieldName[] = "__sg_array_type_name";
        static inline constexpr char s_versionFieldName[] = "__sg_version";

        /**
        * @tparam T Type.
        * @tparam TFormatType Format type.
        * @return Does SerdeSpec<T, TFormatType> has field type_name. Works in compile-time.
        */
        template<typename T, FormatType TFormatType>
        static constexpr bool has_type_name = requires { { SerdeSpec<T, TFormatType>::type_name } -> std::same_as<std::string>; };


        /**
         * @tparam T Type.
         * @tparam TFormatType Format type.
         * @return Type name in SerdeSpec<T, TFormatType>::type_name if field type_name exists. Otherwise, returns empty string.
         */
        template<typename T, FormatType TFormatType>
        static std::string getTypeName() noexcept
        {
            if constexpr(has_type_name<T, TFormatType>)
            {
                return SerdeSpec<T, TFormatType>::type_name();
            }

            return "";
        }
    }
}

#endif // SUNGEARENGINE_SERDE_COMMON_H
