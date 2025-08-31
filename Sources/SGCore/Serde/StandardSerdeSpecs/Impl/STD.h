//
// Created by stuka on 24.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

#include <memory>

namespace SGCore::Serde
{
    // =============================================== impl std::unique_ptr

    template<typename T, FormatType TFormatType>
    template<typename ValueViewT>
    T* SerdeSpec<std::unique_ptr<T>, TFormatType>::getObjectRawPointer(ValueViewT& valueView) noexcept
    {
        return (*valueView.m_data).get();
    }

    template<typename T, FormatType TFormatType>
    template<typename ValueViewT, typename T0>
    void SerdeSpec<std::unique_ptr<T>, TFormatType>::setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept
    {
        *valueView.m_data = std::move(std::unique_ptr<T0>(pointer));
    }

    template<typename T, FormatType TFormatType>
    std::unique_ptr<T> SerdeSpec<std::unique_ptr<T>, TFormatType>::allocateObject(DeserializableValueView<std::unique_ptr<T>, TFormatType>& valueView) noexcept
    {
        return std::make_unique<T>();
    }

    // =============================================== impl std::shared_ptr

    template<typename T, FormatType TFormatType>
    template<typename ValueViewT>
    T* SerdeSpec<std::shared_ptr<T>, TFormatType>::getObjectRawPointer(ValueViewT& valueView) noexcept
    {
        return (*valueView.m_data).get();
    }

    template<typename T, FormatType TFormatType>
    template<typename ValueViewT, typename T0>
    void SerdeSpec<std::shared_ptr<T>, TFormatType>::setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept
    {
        *valueView.m_data = std::move(std::shared_ptr<T0>(pointer));
    }

    template<typename T, FormatType TFormatType>
    std::shared_ptr<T> SerdeSpec<std::shared_ptr<T>, TFormatType>::allocateObject(DeserializableValueView<std::shared_ptr<T>, TFormatType>& valueView) noexcept
    {
        // if we have public access to constructor of T
        if constexpr(requires { T::T(); })
        {
            return std::make_shared<T>();
        }
        else
        {
            // else trying to allocate using raw pointer
            return std::shared_ptr<T>(new T);
        }
    }

    // =============================================== impl T*

    template<typename T, FormatType TFormatType>
    template<typename ValueViewT>
    T* SerdeSpec<T*, TFormatType>::getObjectRawPointer(ValueViewT& valueView) noexcept
    {
        return (*valueView.m_data);
    }

    template<typename T, FormatType TFormatType>
    template<typename ValueViewT, typename T0>
    void SerdeSpec<T*, TFormatType>::setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept
    {
        if(*valueView.m_data)
        {
            delete *valueView.m_data;
            *valueView.m_data = nullptr;
        }
        *valueView.m_data = pointer;
    }

    template<typename T, FormatType TFormatType>
    T* SerdeSpec<T*, TFormatType>::allocateObject(DeserializableValueView<T*, TFormatType>& valueView) noexcept
    {
        return new T();
    }

    // =============================================== impl T const*

    template<typename T, FormatType TFormatType>
    template<typename ValueViewT>
    const T* SerdeSpec<T const*, TFormatType>::getObjectRawPointer(ValueViewT& valueView) noexcept
    {
        return (*valueView.m_data);
    }

    template<typename T, FormatType TFormatType>
    template<typename ValueViewT, typename T0>
    void SerdeSpec<T const*, TFormatType>::setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept
    {
        if(*valueView.m_data)
        {
            delete *valueView.m_data;
            *valueView.m_data = nullptr;
        }
        *valueView.m_data = pointer;
    }

    template<typename T, FormatType TFormatType>
    T* SerdeSpec<T const*, TFormatType>::allocateObject(DeserializableValueView<T const*, TFormatType>& valueView) noexcept
    {
        return new T();
    }

    // =============================================== impl std::optional

    template<typename T, FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<std::optional<T>, TFormatType>::serialize(SerializableValueView<const std::optional<T>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        if(!valueView.m_data->has_value())
        {
            valueView.getValueContainer().setAsNull();
            return;
        }

        valueView.getValueContainer().addMember("value", valueView.m_data->value(), std::forward<SharedDataT>(sharedData)...);
    }

    template<typename T, FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<std::optional<T>, TFormatType>::deserialize(DeserializableValueView<std::optional<T>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        if(valueView.getValueContainer().isNull())
        {
            *valueView.m_data = std::nullopt;
            return;
        }

        auto data = valueView.getValueContainer().template getMember<T>("value", std::forward<SharedDataT>(sharedData)...);
        if(data)
        {
            (*valueView.m_data) = std::move(*data);
        }
    }

    // =============================================== impl std::vector

    template<typename T, FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<std::vector<T>, TFormatType>::serialize(SerializableValueView<const std::vector<T>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        valueView.getValueContainer().setAsArray();

        for(const auto& v : *valueView.m_data)
        {
            valueView.getValueContainer().pushBack(v, std::forward<SharedDataT>(sharedData)...);
        }
    }

    template<typename T, FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<std::vector<T>, TFormatType>::deserialize(DeserializableValueView<std::vector<T>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        *valueView.m_data = valueView.getValueContainer().template getAsArray<T>(std::forward<SharedDataT>(sharedData)...);
    }

    // =============================================== impl std::array

    template<typename T, size_t Size, FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<std::array<T, Size>, TFormatType>::serialize(SerializableValueView<const std::array<T, Size>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        valueView.getValueContainer().setAsArray();

        for(const auto& v : *valueView.m_data)
        {
            valueView.getValueContainer().pushBack(v, std::forward<SharedDataT>(sharedData)...);
        }
    }

    template<typename T, size_t Size, FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<std::array<T, Size>, TFormatType>::deserialize(DeserializableValueView<std::array<T, Size>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        auto vec = valueView.getValueContainer().template getAsArray<T>(std::forward<SharedDataT>(sharedData)...);
        for(size_t i = 0; i < Size; ++i)
        {
            (*valueView.m_data)[i] = std::move(vec[i]);
        }
    }

    // =============================================== impl std::unordered_set

    template<typename T, typename HashT, typename EqualT, FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<std::unordered_set<T, HashT, EqualT>, TFormatType>::serialize(SerializableValueView<const collection_t, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        valueView.getValueContainer().setAsArray();

        for(const auto& v : *valueView.m_data)
        {
            valueView.getValueContainer().pushBack(v, std::forward<SharedDataT>(sharedData)...);
        }
    }

    template<typename T, typename HashT, typename EqualT, FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<std::unordered_set<T, HashT, EqualT>, TFormatType>::deserialize(DeserializableValueView<collection_t, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        auto valuesVec = valueView.getValueContainer().template getAsArray<T>(std::forward<SharedDataT>(sharedData)...);
        for(auto&& v : valuesVec)
        {
            valueView.m_data->emplace(std::forward<T>(v));
        }
    }

    // =============================================== impl std::list

    template<typename T, FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<std::list<T>, TFormatType>::serialize(SerializableValueView<const std::list<T>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        valueView.getValueContainer().setAsArray();

        for(const auto& v : *valueView.m_data)
        {
            valueView.getValueContainer().pushBack(v, std::forward<SharedDataT>(sharedData)...);
        }
    }

    template<typename T, FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<std::list<T>, TFormatType>::deserialize(DeserializableValueView<std::list<T>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        *valueView.m_data = valueView.getValueContainer().template getAsArray<T, std::list<T>>(std::forward<SharedDataT>(sharedData)...);
    }

    // =============================================== impl std::basic_string

    template<typename CharT, FormatType TFormatType>
    void SerdeSpec<std::basic_string<CharT>, TFormatType>::serialize(SerializableValueView<const std::basic_string<CharT>, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().setAsString(*valueView.m_data);
    }

    template<typename CharT, FormatType TFormatType>
    void SerdeSpec<std::basic_string<CharT>, TFormatType>::deserialize(DeserializableValueView<std::basic_string<CharT>, TFormatType>& valueView) noexcept
    {
        *valueView.m_data = valueView.getValueContainer().template getAsString<CharT>();
    }

    // =============================================== impl std::unordered_map

    template<typename KeyT, typename ValueT, FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<std::unordered_map<KeyT, ValueT>, TFormatType>::serialize(SerializableValueView<const std::unordered_map<KeyT, ValueT>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        for(auto& [key, value] : *valueView.m_data)
        {
            std::string resultKey;
            if constexpr(std::is_convertible_v<KeyT, std::string>)
            {
                resultKey = key;
            }
            else if constexpr(requires {
                resultKey = std::to_string(key);
            })
            {
                resultKey = std::to_string(key);
            }
            else if constexpr(std::is_enum_v<KeyT>)
            {
                resultKey = std::to_string(std::to_underlying(key));
            }
            else
            {
                static_assert(always_false<KeyT>::value, "KeyT in std::unordered_map can not be casted to std::string.");
            }

            valueView.getValueContainer().addMember(resultKey, value, std::forward<SharedDataT>(sharedData)...);
        }
    }

    template<typename KeyT, typename ValueT, FormatType TFormatType>
    template<typename... SharedDataT>
    void SerdeSpec<std::unordered_map<KeyT, ValueT>, TFormatType>::deserialize(DeserializableValueView<std::unordered_map<KeyT, ValueT>, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
    {
        for(auto it = valueView.getValueContainer().memberBegin(); it != valueView.getValueContainer().memberEnd(); ++it)
        {
            const auto val = valueView.getValueContainer().template getMember<ValueT>(it, std::forward<SharedDataT>(sharedData)...);

            if(val)
            {
                const std::string memberName = valueView.getValueContainer().getMemberName(it);
                KeyT resultKey = fromString<KeyT>(memberName);

                (*valueView.m_data)[resultKey] = *val;
            }
        }
    }

    // =============================================== impl std::filesystem::path

    template<FormatType TFormatType>
    void SerdeSpec<std::filesystem::path, TFormatType>::serialize(SerializableValueView<const std::filesystem::path, TFormatType>& valueView) noexcept
    {
        const std::string u8Path = SGCore::Utils::toUTF8(valueView.m_data->u16string());
        valueView.getValueContainer().setAsString(u8Path);
    }

    template<FormatType TFormatType>
    void SerdeSpec<std::filesystem::path, TFormatType>::deserialize(DeserializableValueView<std::filesystem::path, TFormatType>& valueView) noexcept
    {
        const std::u16string tmpPath = valueView.getValueContainer().template getAsString<char16_t>();
        *valueView.m_data = tmpPath;
    }

    // =============================================== impl bool

    template<FormatType TFormatType>
    void SerdeSpec<bool, TFormatType>::serialize(SerializableValueView<const bool, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().setAsBool(*valueView.m_data);
    }

    template<FormatType TFormatType>
    void SerdeSpec<bool, TFormatType>::deserialize(DeserializableValueView<bool, TFormatType>& valueView) noexcept
    {
        *valueView.m_data = valueView.getValueContainer().getAsBool();
    }
}