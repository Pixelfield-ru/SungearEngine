//
// Created by stuka on 20.09.2024.
//

#ifndef SUNGEARENGINE_SERDE_DEFINES_H
#define SUNGEARENGINE_SERDE_DEFINES_H

#include "SGCore/Utils/Macroses.h"

#define sg_serde_define_type_name(n)            \
static const std::string& type_name() noexcept  \
{                                               \
    static const std::string name = n;          \
    return name;                                \
}

#define sg_serde_as_friend()                                    \
template<typename T, SGCore::Serde::FormatType TFormatType>     \
friend struct SGCore::Serde::SerdeSpec;                         \
                                                                \
friend struct SGCore::Serde::Serializer;                        \
                                                                \
template<SGCore::Serde::FormatType TFormatType>                 \
friend struct SGCore::Serde::DeserializableValueContainer;      \
template<SGCore::Serde::FormatType TFormatType>                 \
friend struct SGCore::Serde::SerializableValueContainer;


#define sg_predeclare_serde()                           \
namespace SGCore::Serde                                 \
{                                                       \
    enum class FormatType;                              \
                                                        \
    template<FormatType>                                \
    struct FormatInfo;                                  \
                                                        \
    template<typename T, FormatType TFormatType>        \
    struct SerdeSpec;                                   \
                                                        \
    struct Serializer;                                  \
                                                        \
    template<FormatType TFormatType>                    \
    struct DeserializableValueContainer;                \
    template<FormatType TFormatType>                    \
    struct SerializableValueContainer;                  \
                                                        \
    template<typename T, FormatType TFormatType>        \
    struct SerializableValueView;                       \
                                                        \
    template<typename T, FormatType TFormatType>        \
    struct DeserializableValueView;                     \
}

#define sg_validate_serdespec_supported_formats(CurrentFormat, ...) static_assert(SGCore::contains_obj<CurrentFormat, __VA_ARGS__>::value, "This TFormatType is not supported in current SerdeSpec.");

#define SG_SERDE_DECLARE_EXTERNAL_CONNECTION(Base, Derived, SerdeName) \
template<SGCore::Serde::FormatType TFormatType, typename... SharedDataT> \
struct SG_CONCAT(SerdeName, _Serializer) final : SGCore::Serde::IExternalSerializer<TFormatType, SharedDataT...> \
{ \
    bool invoke(const std::byte* thisTypeObject, SGCore::Serde::SerializableValueContainer<TFormatType>& container, const std::string& version, bool& isDiscarded, SharedDataT&&... sharedData) noexcept \
    { \
        const auto* typed = reinterpret_cast<const Base*>(thisTypeObject); \
\
        { \
            const auto& serdeStorage = SGCore::Serde::ExternalSerializersStorage<Derived>::storage<TFormatType, SharedDataT...>(); \
            for(const auto& serializer : serdeStorage) \
            { \
                if(serializer->invoke(thisTypeObject, container, version, isDiscarded, std::forward<SharedDataT>(sharedData)...)) return true; \
            } \
        } \
\
        { \
            /* trying to invoke serialize function without shared data (default behaviour if call to serialize with share data provides failure) */ \
            const auto& serdeStorage = SGCore::Serde::ExternalSerializersStorage<Derived>::storage<TFormatType>(); \
            for(const auto& serializer : serdeStorage) \
            { \
                if(serializer->invoke(thisTypeObject, container, version, isDiscarded)) return true; \
            } \
        } \
\
        const auto* asDerived = dynamic_cast<const Derived*>(typed); \
        if(!asDerived) return false; \
\
        SGCore::Serde::SerializableValueView<const Derived, TFormatType> valueView {}; \
        valueView.container() = container; \
        valueView.m_version = version; \
        valueView.m_data = asDerived; \
\
        valueView.container().setTypeName(SGCore::Serde::Detail::getTypeName<Derived, TFormatType>()); \
\
        SGCore::Serde::Serializer::serializeWithDynamicChecks(valueView, true, std::forward<SharedDataT>(sharedData)...); \
\
        return true; \
    } \
\
}; \
\
template<SGCore::Serde::FormatType TFormatType, typename... SharedDataT> \
struct SG_CONCAT(SerdeName, _Deserializer) final : SGCore::Serde::IExternalDeserializer<TFormatType, SharedDataT...> \
{ \
    std::byte* invoke(SGCore::Serde::DeserializableValueContainer<TFormatType>& container, const std::string& version, SharedDataT&&... sharedData) noexcept \
    { \
        { \
            const auto& serdeStorage = SGCore::Serde::ExternalDeserializersStorage<Derived>::storage<TFormatType, SharedDataT...>(); \
            for(const auto& deserializer : serdeStorage) \
            { \
                std::byte* deserializedObject = deserializer->invoke(container, version, std::forward<SharedDataT>(sharedData)...); \
                if(deserializedObject) return deserializedObject; \
            } \
        } \
        \
        { \
            /* trying to invoke serialize function without shared data (default behaviour if call to serialize with share data provides failure) */ \
            const auto& serdeStorage = SGCore::Serde::ExternalDeserializersStorage<Derived>::storage<TFormatType>(); \
            for(const auto& deserializer : serdeStorage) \
            { \
                std::byte* deserializedObject = deserializer->invoke(container, version); \
                if(deserializedObject) return deserializedObject; \
            } \
        } \
\
        SGCore::Serde::DeserializableValueView<Derived, TFormatType> valueView {}; \
        valueView.container() = container; \
        valueView.m_version = version; \
\
        if(container.getTypeName() != SGCore::Serde::Detail::getTypeName<Derived, TFormatType>()) \
        { \
            /* trying to call deserialization of Derived derived types, that declared in DerivedTypes of SerdeSpec of Derived */ \
            SGCore::Serde::Serializer::deserializeWithDynamicChecks(valueView, true, std::forward<SharedDataT>(sharedData)...); \
            return reinterpret_cast<std::byte*>(valueView.m_data); \
        } \
\
        Derived* derivedObject {}; \
        if constexpr(!std::is_abstract_v<Derived> && std::is_default_constructible_v<Derived>) \
        { \
            derivedObject = new Derived(); \
        } \
        else \
        { \
            if constexpr(requires { SGCore::Serde::SerdeSpec<Derived, TFormatType>::allocateObject; }) \
            { \
                derivedObject = SGCore::Serde::SerdeSpec<Derived, TFormatType>::allocateObject(valueView); \
            } \
            else \
            { \
                /* can not use static_assert because this branch is not in compile time context */ \
                return nullptr; \
            } \
        } \
\
        valueView.m_data = derivedObject; \
        /* deserializing value */ \
        SGCore::Serde::Serializer::deserializeWithDynamicChecks(valueView, false, std::forward<SharedDataT>(sharedData)...); \
\
        return reinterpret_cast<std::byte*>(derivedObject); \
    } \
};

#define SG_SERDE_REGISTER_EXTERNAL_SERIALIZER_FOR_FORMAT_IMPL(Base, Derived, SerdeName, FormatType, Line, ...) \
auto SG_CONCAT(_SG_DECLARE_SERIALIZER_SPEC_, Line) = []() { \
    static SG_CONCAT(SerdeName, _Serializer)<FormatType __VA_OPT__(,) __VA_ARGS__> serializer; \
    SGCore::Serde::ExternalSerializersStorage<Base>::addSerializer<FormatType __VA_OPT__(,) __VA_ARGS__>(&serializer); \
    return true; \
}();

#define SG_SERDE_REGISTER_EXTERNAL_SERIALIZER_FOR_FORMAT(Base, Derived, SerdeName, FormatType, ...) \
    SG_SERDE_REGISTER_EXTERNAL_SERIALIZER_FOR_FORMAT_IMPL(Base, Derived, SerdeName, FormatType, __LINE__, __VA_ARGS__)

#define SG_SERDE_REGISTER_EXTERNAL_SERIALIZER(Base, Derived, SerdeName, ...) \
    SG_SERDE_REGISTER_EXTERNAL_SERIALIZER_FOR_FORMAT_IMPL(Base, Derived, SerdeName, SGCore::Serde::FormatType::JSON, SG_CONCAT(__LINE__, _JSON), __VA_ARGS__) \
    SG_SERDE_REGISTER_EXTERNAL_SERIALIZER_FOR_FORMAT_IMPL(Base, Derived, SerdeName, SGCore::Serde::FormatType::BSON, SG_CONCAT(__LINE__, _BSON), __VA_ARGS__) \
    SG_SERDE_REGISTER_EXTERNAL_SERIALIZER_FOR_FORMAT_IMPL(Base, Derived, SerdeName, SGCore::Serde::FormatType::YAML, SG_CONCAT(__LINE__, _YAML), __VA_ARGS__)

#define SG_SERDE_REGISTER_EXTERNAL_DESERIALIZER_FOR_FORMAT_IMPL(Base, Derived, SerdeName, FormatType, Line, ...) \
auto SG_CONCAT(_SG_DECLARE_DESERIALIZER_SPEC_, Line) = []() { \
    static SG_CONCAT(SerdeName, _Deserializer)<FormatType __VA_OPT__(,) __VA_ARGS__> deserializer; \
    SGCore::Serde::ExternalDeserializersStorage<Base>::addDeserializer<FormatType __VA_OPT__(,) __VA_ARGS__>(&deserializer); \
    return true; \
}();

#define SG_SERDE_REGISTER_EXTERNAL_DESERIALIZER_FOR_FORMAT(Base, Derived, SerdeName, FormatType, ...) \
SG_SERDE_REGISTER_EXTERNAL_DESERIALIZER_FOR_FORMAT_IMPL(Base, Derived, SerdeName, FormatType, __LINE__, __VA_ARGS__)

#define SG_SERDE_REGISTER_EXTERNAL_DESERIALIZER(Base, Derived, SerdeName, ...) \
SG_SERDE_REGISTER_EXTERNAL_DESERIALIZER_FOR_FORMAT_IMPL(Base, Derived, SerdeName, SGCore::Serde::FormatType::JSON, SG_CONCAT(__LINE__, _JSON), __VA_ARGS__) \
// SG_SERDE_REGISTER_EXTERNAL_DESERIALIZER_FOR_FORMAT_IMPL(Base, Derived, SerdeName, SGCore::Serde::FormatType::BSON, SG_CONCAT(__LINE__, _BSON), __VA_ARGS__) \
// SG_SERDE_REGISTER_EXTERNAL_DESERIALIZER_FOR_FORMAT_IMPL(Base, Derived, SerdeName, SGCore::Serde::FormatType::YAML, SG_CONCAT(__LINE__, _YAML), __VA_ARGS__)

#endif // SUNGEARENGINE_SERDE_DEFINES_H
