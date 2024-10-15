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
    template<typename T, FormatType TFormatType>        \
    struct SerdeSpec;                                   \
                                                        \
    struct Serializer;                                  \
                                                        \
    template<FormatType TFormatType>                    \
    struct DeserializableValueContainer;                \
    template<FormatType TFormatType>                    \
    struct SerializableValueContainer;                  \
}

#endif // SUNGEARENGINE_SERDE_DEFINES_H
