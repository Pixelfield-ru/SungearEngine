//
// Created by stuka on 24.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

#include <memory>

namespace SGCore::Serde
{
    // ======================================================== std::unique_ptr FWD

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<std::unique_ptr<T>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = Serde::getTypeName<T, TFormatType>();
        static inline constexpr bool is_pointer_type = true;
        using element_type = T;

        template<typename ValueViewT>
        static T* getObjectRawPointer(ValueViewT& valueView) noexcept;

        template<typename ValueViewT, typename T0>
        static void setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept;

        static std::unique_ptr<T> allocateObject(DeserializableValueView<std::unique_ptr<T>, TFormatType>& valueView) noexcept;
    };

    // ======================================================== std::shared_ptr FWD

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<std::shared_ptr<T>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = Serde::getTypeName<T, TFormatType>();
        static inline constexpr bool is_pointer_type = true;
        using element_type = T;

        template<typename ValueViewT>
        static T* getObjectRawPointer(ValueViewT& valueView) noexcept;

        template<typename ValueViewT, typename T0>
        static void setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept;

        static std::shared_ptr<T> allocateObject(DeserializableValueView<std::shared_ptr<T>, TFormatType>& valueView) noexcept;
    };

    // ======================================================== T* FWD

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<T*, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = Serde::getTypeName<T, TFormatType>();
        static inline constexpr bool is_pointer_type = true;
        using element_type = T;

        template<typename ValueViewT>
        static T* getObjectRawPointer(ValueViewT& valueView) noexcept;

        template<typename ValueViewT, typename T0>
        static void setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept;

        static T* allocateObject(DeserializableValueView<T*, TFormatType>& valueView) noexcept;
    };

    // ======================================================== T const* FWD

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<T const*, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        static inline const std::string type_name = Serde::getTypeName<T, TFormatType>();
        static inline constexpr bool is_pointer_type = true;
        using element_type = T;

        template<typename ValueViewT>
        static const T* getObjectRawPointer(ValueViewT& valueView) noexcept;

        template<typename ValueViewT, typename T0>
        static void setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept;

        static T* allocateObject(DeserializableValueView<T const*, TFormatType>& valueView) noexcept;
    };

    // ======================================================== std::optional FWD

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<std::optional<T>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("std::optional")
        static inline constexpr bool is_pointer_type = false;
        using element_type = T;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const std::optional<T>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept;

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<std::optional<T>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept;
    };

    // ======================================================== FloatingT FWD and impl (i can not separate definition and implementation because of MSVC bug i think)

    template<std::floating_point FloatingT, FormatType TFormatType>
    struct SerdeSpec<FloatingT, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("float")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const FloatingT, TFormatType>& valueView) noexcept
        {
            const FloatingT finalValue = std::isfinite(*valueView.m_data) ? *valueView.m_data : 0.0;
            valueView.container().setAsFloat(*valueView.m_data);
        }

        static void deserialize(DeserializableValueView<FloatingT, TFormatType>& valueView) noexcept
        {
            *valueView.m_data = valueView.container().getAsFloat();
        }
    };

    // ======================================================== IntegerT FWD and impl (i can not separate definition and implementation because of MSVC bug i think)

    template<std::integral IntegerT, FormatType TFormatType>
    struct SerdeSpec<IntegerT, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("int")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const IntegerT, TFormatType>& valueView) noexcept
        {
            const IntegerT finalVal = std::isfinite(*valueView.m_data) ? *valueView.m_data : 0;
            valueView.container().setAsInt64(finalVal);
        }

        static void deserialize(DeserializableValueView<IntegerT, TFormatType>& valueView) noexcept
        {
            *valueView.m_data = valueView.container().getAsInt64();
        }
    };

    // ======================================================== std::vector FWD

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<std::vector<T>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("std::vector")
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const std::vector<T>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept;

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<std::vector<T>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept;
    };

    // ======================================================== std::array FWD

    template<typename T, size_t Size, FormatType TFormatType>
    struct SerdeSpec<std::array<T, Size>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("std::array")
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const std::array<T, Size>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept;

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<std::array<T, Size>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept;
    };

    // ======================================================== std::unordered_set FWD

    template<typename T, typename HashT, typename EqualT, FormatType TFormatType>
    struct SerdeSpec<std::unordered_set<T, HashT, EqualT>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        using collection_t = std::unordered_set<T, HashT, EqualT>;

        SG_SERDE_DEFINE_TYPE_NAME("std::unordered_set")
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const collection_t, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept;

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<collection_t, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept;
    };

    // ======================================================== std::list FWD

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<std::list<T>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("std::list")
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const std::list<T>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept;

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<std::list<T>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept;
    };

    // ======================================================== std::basic_string FWD

    template<typename CharT, FormatType TFormatType>
    struct SerdeSpec<std::basic_string<CharT>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("std::basic_string")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const std::basic_string<CharT>, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<std::basic_string<CharT>, TFormatType>& valueView) noexcept;
    };

    // ======================================================== std::unordered_map FWD

    /**
     * KeyT REQUIRES AN IMPLICIT CONVERSION OPERATOR TO std::string OR OTHER TYPES FROM
     * WHICH std::string CAN BE CONSTRUCTED OR WHICH CAN BE IMPLICITLY CONVERTED TO std::string
     * OR CAN BE CONVERTED TO std::string USING std::to_string.
     *
    **/
    template<typename KeyT, typename ValueT, FormatType TFormatType>
    struct SerdeSpec<std::unordered_map<KeyT, ValueT>, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("std::unordered_map")
        static inline constexpr bool is_pointer_type = false;

        template<typename... SharedDataT>
        static void serialize(SerializableValueView<const std::unordered_map<KeyT, ValueT>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept;

        template<typename... SharedDataT>
        static void deserialize(DeserializableValueView<std::unordered_map<KeyT, ValueT>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept;
    };

    // ======================================================== std::filesystem::path FWD

    template<FormatType TFormatType>
    struct SerdeSpec<std::filesystem::path, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("std::filesystem::path")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const std::filesystem::path, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<std::filesystem::path, TFormatType>& valueView) noexcept;
    };

    // ======================================================== bool FWD

    template<FormatType TFormatType>
    struct SerdeSpec<bool, TFormatType> :
            BaseTypes<>,
            DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("bool")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const bool, TFormatType>& valueView) noexcept;

        static void deserialize(DeserializableValueView<bool, TFormatType>& valueView) noexcept;
    };

    // ======================================================== enum T FWD (i can not separate definition and implementation because of MSVC bug i think)

    template<typename T, FormatType TFormatType>
    requires(std::is_enum_v<T>)
    struct SerdeSpec<T, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        SG_SERDE_DEFINE_TYPE_NAME("enum")
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const T, TFormatType>& valueView) noexcept
        {
            valueView.container().setAsInt64(std::to_underlying(*valueView.m_data));
        }

        static void deserialize(DeserializableValueView<T, TFormatType>& valueView) noexcept
        {
            *valueView.m_data = (T) valueView.container().getAsInt64();
        }
    };
}
