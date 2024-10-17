//
// Created by stuka on 20.09.2024.
//

#ifndef SUNGEARENGINE_SERDE_DEFINES_H
#define SUNGEARENGINE_SERDE_DEFINES_H

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

#endif // SUNGEARENGINE_SERDE_DEFINES_H
