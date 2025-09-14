//
// Created by stuka on 20.09.2024.
//

#ifndef SUNGEARENGINE_SERDE_DEFINES_H
#define SUNGEARENGINE_SERDE_DEFINES_H

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

#define SG_SERDE_ARG_COUNT(...) \
SG_SERDE_ARG_COUNT_IMPL(0, ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define SG_SERDE_ARG_COUNT_IMPL(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N

#define SG_SERDE_CONCAT(a, b) SG_SERDE_CONCAT_IMPL(a, b)
#define SG_SERDE_CONCAT_IMPL(a, b) a##b

#define SG_SERDE_DECLARE_EXTERNAL_DERIVED(Base, Derived, SerdeName) \
template<SGCore::Serde::FormatType TFormatType, typename... SharedDataT> \
struct SerdeName final : SGCore::Serde::IDynamicSerde<TFormatType, SharedDataT...> \
{ \
    bool invokeSerialize(const std::byte* thisTypeObject, SGCore::Serde::SerializableValueContainer<TFormatType>& container, const std::string& version, bool& isDiscarded, SharedDataT&&... sharedData) noexcept \
    { \
        const auto* typed = reinterpret_cast<const Base*>(thisTypeObject); \
\
        const auto& derivedSerializers = SGCore::Serde::DynamicSerdeStorage<Derived>::dynamicSerializers<TFormatType, SharedDataT...>(); \
        if(!derivedSerializers.empty()) \
        { \
            for(const auto& serializer : derivedSerializers) \
            { \
                if(serializer->invokeSerialize(thisTypeObject, container, version, isDiscarded, std::forward<SharedDataT>(sharedData)...)) return true; \
            } \
        } \
\
        const auto* asDerived = dynamic_cast<const Derived*>(typed); \
        if(!asDerived) return false; \
\
        /* todo: if(SGCore::Serde::Serializer::serializeWithDynamicChecks()) return true; */ \
\
        using base_types = SGCore::add_type_to_container_t<typename SGCore::Serde::Serializer::collect_all_base_types<Derived, TFormatType>::type, Derived>; \
        SGCore::for_types([&](auto wrapper) { \
            using base_type = typename decltype(wrapper)::type;\
\
            const auto* base = reinterpret_cast<const base_type*>(thisTypeObject); \
\
            SGCore::Serde::SerializableValueView<const base_type, TFormatType> valueView {}; \
            valueView.container() = container; \
            valueView.m_version = version; \
            valueView.m_data = base; \
\
            SGCore::Serde::Serializer::invokeSerdeSpecSerialize(valueView, std::forward<SharedDataT>(sharedData)...); \
\
            if(valueView.isDiscarded()) \
            { \
                isDiscarded = true; \
            } \
        }, base_types{}); \
\
        container.setTypeName(SGCore::Serde::Detail::getTypeName<Derived, TFormatType>()); \
\
        return true; \
    } \
};

#define SG_SERDE_REGISTER_DYNAMIC_SERDE_FOR_FORMAT_IMPL(Base, Derived, SerdeName, FormatType, Line, ...) \
auto SG_SERDE_CONCAT(_SG_DECLARE_SERIALIZER_SPEC_, Line) = []() { \
    static SerdeName<FormatType __VA_OPT__(,) __VA_ARGS__> serializer; \
    SGCore::Serde::DynamicSerdeStorage<Base>::dynamicSerializers<FormatType __VA_OPT__(,) __VA_ARGS__>().push_back(&serializer); \
    return true; \
}();

#define SG_SERDE_REGISTER_DYNAMIC_SERDE_FOR_FORMAT(Base, Derived, SerdeName, FormatType, ...) \
    SG_SERDE_REGISTER_DYNAMIC_SERDE_FOR_FORMAT_IMPL(Base, Derived, SerdeName, FormatType, __LINE__, __VA_ARGS__)

#define SG_SERDE_REGISTER_DYNAMIC_SERDE(Base, Derived, SerdeName, ...) \
    SG_SERDE_REGISTER_DYNAMIC_SERDE_FOR_FORMAT_IMPL(Base, Derived, SerdeName, SGCore::Serde::FormatType::JSON, SG_SERDE_CONCAT(__LINE__, _JSON), __VA_ARGS__) \
    SG_SERDE_REGISTER_DYNAMIC_SERDE_FOR_FORMAT_IMPL(Base, Derived, SerdeName, SGCore::Serde::FormatType::BSON, SG_SERDE_CONCAT(__LINE__, _BSON), __VA_ARGS__) \
    SG_SERDE_REGISTER_DYNAMIC_SERDE_FOR_FORMAT_IMPL(Base, Derived, SerdeName, SGCore::Serde::FormatType::YAML, SG_SERDE_CONCAT(__LINE__, _YAML), __VA_ARGS__)

#endif // SUNGEARENGINE_SERDE_DEFINES_H
