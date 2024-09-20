//
// Created by stuka on 20.09.2024.
//

#ifndef SUNGEARENGINE_SERDE_DEFINES_H
#define SUNGEARENGINE_SERDE_DEFINES_H

#define sg_serdespec_as_friend() template<typename T, SGCore::Serde::FormatType TFormatType> friend struct SGCore::Serde::SerdeSpec;
#define sg_predeclare_serdespec() namespace SGCore::Serde   \
{                                                   \
    enum class FormatType;                          \
    template<typename T, FormatType TFormatType>    \
    struct SerdeSpec;                               \
}

#endif // SUNGEARENGINE_SERDE_DEFINES_H
