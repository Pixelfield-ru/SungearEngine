//
// Created by Ilya on 14.08.2024.
//

#ifndef SUNGEARENGINE_NEWSERIALIZER_H
#define SUNGEARENGINE_NEWSERIALIZER_H

#include "SGCore/Utils/TypeTraits.h"

namespace SGCore::NewSerde
{
    enum class FormatType
    {
        JSON,
        BSON,
        YAML
    };

    // DO NOT STORE ANYWHERE
    // DOES NOT OWN ANYTHING
    template<typename T>
    struct IDeserializableValueView
    {

    };

    // DO NOT STORE ANYWHERE
    // DOES NOT OWN ANYTHING
    template<typename T>
    struct ISerializableValueView
    {
        const T* m_data { };
    };

    template<typename T>
    struct JSONSerializableValueView final : ISerializableValueView<T>
    {
        friend struct Serializer;

    protected:
        rapidjson::Document* m_document { };
    };

    // ==============================================================================
    // ==============================================================================
    // ==============================================================================

    // USE THAT STRUCTS TO DEFINE BASE CLASSES OF SERIALIZABLE TYPE AND DERIVED CLASSES
    template<typename... Cls>
    struct BaseTypes
    {
        static constexpr std::uint64_t base_classes_count = sizeof...(Cls);

        template<std::int64_t Idx>
        using get_base_type = extract<Idx, Cls...>::type;
    };

    template<typename... Cls>
    struct DerivedTypes
    {
        static constexpr std::uint64_t derived_classes_count = sizeof...(Cls);

        template<std::int64_t Idx>
        using get_derived_type = extract<Idx, Cls...>::type;
    };

    // ==============================================================================
    // ==============================================================================
    // ==============================================================================

    template<typename T>
    struct SerdeSpec : BaseTypes<>, DerivedTypes<>
    {
        static_assert(always_false<T>::value,
                      "This type is not serializable. Please, implement specialization of SerializerSpec for this type.");

        // the type name of T
        static inline const std::string type_name = "unknown";
        // is T is pointer type
        static inline constexpr bool is_pointer_type = false;

        static void serialize(ISerializableValueView<T>& valueView,
                              FormatType formatType)
        {

        }

        static void deserialize(IDeserializableValueView<T>& valueView,
                                FormatType formatType)
        {

        }
    };

    struct Serializer
    {
        template<typename T>
        static void toFormat(T& value, FormatType formatType = m_defaultFormatType) noexcept
        {
            switch (m_defaultFormatType)
            {
                case FormatType::JSON:
                {
                    rapidjson::Document document;
                    document.SetObject();

                    toJSON(document, value);
                    break;
                }
                case FormatType::BSON:
                    break;
                case FormatType::YAML:
                    break;
            }
        }

        template<typename T>
        static void serializeMember(ISerializableValueView<T>& parent,
                                    const std::string& varName,
                                    const T& value) noexcept
        {

        }

    private:
        static FormatType m_defaultFormatType;

        template<typename T>
        static void toJSON(rapidjson::Document& toDocument,
                           const T& value)
        {

        }
    };
}

#endif //SUNGEARENGINE_NEWSERIALIZER_H
