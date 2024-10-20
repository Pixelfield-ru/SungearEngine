//
// Created by stuka on 20.10.2024.
//

#ifndef SUNGEARENGINE_JSON_SERIALIZERFORWARD_H
#define SUNGEARENGINE_JSON_SERIALIZERFORWARD_H

#include "SGCore/Serde/Common.h"

template<>
class SerializerImpl<FormatType::JSON>
{
    friend struct Serializer;

    /**
     * Converts object to JSON format
     * @tparam T - Serializable type.
     * @param toDocument
     * @param value
     * @return
     */
    template<custom_derived_types_t CustomDerivedTypes = custom_derived_types<>, typename... SharedDataT, typename T>
    static std::string to(const T& value,
                          SharedDataT&& ... sharedData) noexcept;

    /**
     * Converts JSON document to object.
     * @tparam T
     * @param toDocument
     * @param value
     * @return
     */
    template<custom_derived_types_t CustomDerivedTypes = custom_derived_types<>, typename T, typename... SharedDataT>
    static void from(const std::string& formattedText,
                     std::string& outputLog,
                     T& outValue,
                     SharedDataT&& ... sharedData) noexcept;
};

#endif // SUNGEARENGINE_JSON_SERIALIZERFORWARD_H
