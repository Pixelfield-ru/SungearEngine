//
// Created by stuka on 31.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

namespace SGCore::Serde
{
    // ======================================================== glm::vec FWD

    template<glm::length_t Length, typename T, glm::qualifier Qualifier, FormatType TFormatType>
    struct SerdeSpec<glm::vec<Length, T, Qualifier>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("glm::vec")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const glm::vec<Length, T, Qualifier>, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<glm::vec<Length, T, Qualifier>, TFormatType>& valueView) noexcept;
    };

    // ======================================================== glm::qua FWD

    template<typename T, glm::qualifier Qualifier, FormatType TFormatType>
    struct SerdeSpec<glm::qua<T, Qualifier>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("glm::qua")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const glm::qua<T, Qualifier>, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<glm::qua<T, Qualifier>, TFormatType>& valueView) noexcept;
    };

    // ======================================================== glm::mat FWD

    template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Qualifier, FormatType TFormatType>
    struct SerdeSpec<glm::mat<C, R, T, Qualifier>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("glm::mat")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const glm::mat<C, R, T, Qualifier>, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<glm::mat<C, R, T, Qualifier>, TFormatType>& valueView) noexcept;
    };
}