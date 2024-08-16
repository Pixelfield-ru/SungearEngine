//
// Created by Ilya on 14.08.2024.
//

#ifndef SUNGEARENGINE_NEWSERIALIZER_H
#define SUNGEARENGINE_NEWSERIALIZER_H

#include "SGCore/Utils/TypeTraits.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::NewSerde
{
    enum class FormatType
    {
        JSON,
        BSON,
        YAML
    };

    // DO NOT STORE ANYWHERE. DOES NOT OWN ANYTHING
    template<typename T, FormatType TFormatType>
    struct DeserializableValueView
    {

    };

    /**
     * View of value with format type`s specific members.\n
     * You must implement all next functions and members in your specialization of this struct.\n
     * DO NOT STORE ANYWHERE. DOES NOT OWN ANYTHING.
     * @tparam T
     * @tparam TFormatType
     */
    template<typename T, FormatType TFormatType>
    struct SerializableValueView
    {
        friend struct Serializer;

        static inline constexpr FormatType format_type = TFormatType;

        const T* m_data { };

        template<typename T0>
        void copyFormatPointers(SerializableValueView<T0, TFormatType>& to) noexcept
        {

        }

    private:
        SerializableValueView<T, TFormatType>* m_parent;
    };

    template<typename T>
    struct SerializableValueView<T, FormatType::JSON>
    {
        friend struct Serializer;

        // doing all SerializableValueView (JSON) with different T as friends
        template<typename T0, FormatType TFormatType>
        friend struct SerializableValueView;

        static inline constexpr FormatType format_type = FormatType::JSON;

        const T* m_data { };

        template<typename T0>
        void copyFormatPointers(SerializableValueView<T0, FormatType::JSON>& to) noexcept
        {
            to.m_document = m_document;

            std::printf("called json copyFormatPointersImpl\n");
        }

    private:
        SerializableValueView<T, FormatType::JSON>* m_parent;
        rapidjson::Document* m_document { };
    };

    // ==============================================================================
    // ==============================================================================
    // ==============================================================================

    // USE THAT STRUCTS TO DEFINE BASE CLASSES OF SERIALIZABLE TYPE AND DERIVED CLASSES
    /**
     * Contains base types
     * @tparam Cls
     */
    template<typename... Cls>
    struct BaseTypes
    {
        static constexpr std::uint64_t base_classes_count = sizeof...(Cls);

        template<std::int64_t Idx>
        using get_base_type = extract<Idx, Cls...>::type;
    };

    /**
     * Contains derived types
     * @tparam Cls
     */
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

    /**
     * Primary SerdeSpec structure. Use this structure body as example to implement custom SerdeSpec.\n
     * @tparam T
     */
    template<typename T, FormatType TFormatType>
    struct SerdeSpec : BaseTypes<>, DerivedTypes<>
    {
        static_assert(always_false<T>::value,
                      "This type is not serializable. Please, implement specialization of SerdeSpec for this type.");

        /**
         * The type name of T
         */
        static inline const std::string type_name = "unknown";
        /**
         * Is T is pointer type
         */
        static inline constexpr bool is_pointer_type = false;
        /**
         * OPTIONAL (IF TYPES IS POINTER): type that pointer points to
         */
        using element_type = void;

        /**
         * Serializes only T type members.
         * @param valueView
         */
        static void serialize(SerializableValueView<T, TFormatType>& valueView)
        {

        }

        /**
         * Dserializes only T type members.
         * @param valueView
         */
        static void deserialize(DeserializableValueView<T, TFormatType>& valueView)
        {

        }
    };

    // ================================================================
    // ================================================================
    // ================================================================

    struct Utils
    {
        /**
         * Is SerdeSpec of T provides base types.
         * @tparam T
         * @return
         */
        template<typename T, FormatType TFormatType>
        static consteval bool isBaseTypesProvided()
        {
            return requires {
                SerdeSpec<T, TFormatType>::base_classes_count,
                SerdeSpec<T, TFormatType>::get_base_type;
            };
        }

        /**
        * Is SerdeSpec of T provides derived types.
        * @tparam T
        * @return
        */
        template<typename T, FormatType TFormatType>
        static consteval bool isDerivedTypesProvided()
        {
            return requires {
                SerdeSpec<T, TFormatType>::derived_classes_count,
                SerdeSpec<T, TFormatType>::get_derived_type;
            };
        }
    };

    // ================================================================
    // ================================================================
    // ================================================================

    struct Serializer
    {
        /**
         * Converts object to some format (JSON, BSON, YAML, etc)
         * @tparam T
         * @param value
         * @param formatType
         * @return
         */
        template<typename T>
        static std::string toFormat(T& value, FormatType formatType = m_defaultFormatType) noexcept
        {
            switch (formatType)
            {
                case FormatType::JSON:
                {
                    rapidjson::Document document;
                    document.SetObject();

                    return toJSON(document, value);
                }
                case FormatType::BSON:
                    break;
                case FormatType::YAML:
                    break;
            }

            return "";
        }

        /*template<typename T, FormatType TFormatType>
        static void serializeMember(ISerializableValueView<T, TFormatType>& parent,
                                    const std::string& varName,
                                    const T& value) noexcept
        {

        }*/

    private:
        static inline FormatType m_defaultFormatType = SGCore::NewSerde::FormatType::JSON;

        /**
         * Converts object to JSON format
         * @tparam T
         * @param toDocument
         * @param value
         * @return
         */
        template<typename T>
        static std::string toJSON(rapidjson::Document& toDocument,
                                  const T& value)
        {
            // adding version of document
            toDocument.AddMember("version", 1, toDocument.GetAllocator());

            // adding type name of T
            rapidjson::Value typeNameSectionValue(rapidjson::kStringType);
            typeNameSectionValue.SetString(SerdeSpec<T, FormatType::JSON>::type_name.c_str(),
                                           SerdeSpec<T, FormatType::JSON>::type_name.length());
            toDocument.AddMember("typeName", typeNameSectionValue, toDocument.GetAllocator());

            // creating section that will contain all members of T
            rapidjson::Value valueSectionValue(rapidjson::kObjectType);

            // ==== serialization code

            // creating view of value with format pointers
            SerializableValueView<T, FormatType::JSON> valueView  { };
            valueView.m_data = &value;
            valueView.m_document = &toDocument;

            // serializing value with attempt at dynamic casts to derived types
            serializeWithDynamicChecks<T, FormatType::JSON>(valueView);

            // =======================

            // adding value section to document
            toDocument.AddMember("value", valueSectionValue, toDocument.GetAllocator());

            // converting to string
            rapidjson::StringBuffer stringBuffer;
            stringBuffer.Clear();
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);
            toDocument.Accept(writer);

            return stringBuffer.GetString();
        }

        /**
         * Serializes value with attempt at dynamic casts to derived types.\n
         * If all attempts of casting value to derived types are failed than serializing only T type members and members of all base types.
         * @tparam T
         * @param valueView
         */
        template<typename T, FormatType TFormatType>
        static void serializeWithDynamicChecks(SerializableValueView<T, TFormatType>& valueView)
        {
            if constexpr(SerdeSpec<T, TFormatType>::is_pointer_type) // serializing value using dynamic checks
            {
                // getting element_type that pointer contains
                using ptr_element_type = SerdeSpec<T, TFormatType>::element_type;

                // creating view that contains element_type object
                SerializableValueView<ptr_element_type, TFormatType> tmpView { };
                valueView.template copyFormatPointers<ptr_element_type>(tmpView);
                tmpView.m_data = &(**valueView.m_data);

                // serializing base types
                serializeBaseTypes<ptr_element_type, TFormatType>(tmpView);

                // serializing derived types
                serializeDerivedTypes<ptr_element_type, TFormatType>(tmpView);

                // serializing element_type type members
                SerdeSpec<T, TFormatType>::serialize(valueView);

                return;
            }

            // serialize without dynamic checks (static serialization)

            // serializing base types
            serializeBaseTypes<T, TFormatType>(valueView);

            // serializing only T type members
            SerdeSpec<T, TFormatType>::serialize(valueView);
        }

        // =====================================================================================
        // =====================================================================================
        // =====================================================================================

        /**
         * Serializes base type (BaseT) of parent type (OriginalT)
         * @tparam OriginalT
         * @tparam BaseT
         * @param valueView
         */
        template<typename OriginalT, typename BaseT, FormatType TFormatType>
        static void serializeBaseType(SerializableValueView<OriginalT, TFormatType>& valueView) noexcept
        {
            // converting OriginalT value view to BaseT value view to pass into SerdeSpec
            SerializableValueView<BaseT, TFormatType> tmpView { };
            valueView.template copyFormatPointers<BaseT>(tmpView);
            tmpView.m_data = &(static_cast<const BaseT&>(*valueView.m_data));

            // serialize BaseT`s base types
            serializeBaseTypes<BaseT, TFormatType>(tmpView);

            // passing tmpView with BaseT to SerdeSpec
            SerdeSpec<BaseT, TFormatType>::serialize(tmpView);
        }

        /**
         * Implementation of serializing base types of T. Uses unpacking.
         * @tparam T
         * @tparam Indices
         * @param valueView
         */
        template<typename T, FormatType TFormatType, std::size_t... Indices>
        static void serializeBaseTypesImpl(SerializableValueView<T, TFormatType>& valueView,
                                           std::index_sequence<Indices...>) noexcept
        {
            // unpacking variadic template
            (serializeBaseType<T,
                    typename SerdeSpec<T, TFormatType>::template get_base_type<Indices>,
                    TFormatType>(valueView), ...);
        }

        /**
         * Serializes all base types of T.
         * @tparam T
         * @param valueView
         */
        template<typename T, FormatType TFormatType>
        static void serializeBaseTypes(SerializableValueView<T, TFormatType>& valueView) noexcept
        {
            // serializing base types only if information of them was provided
            if constexpr(Utils::isBaseTypesProvided<T, TFormatType>())
            {
                serializeBaseTypesImpl<T, TFormatType>
                        (valueView, std::make_index_sequence<SerdeSpec<T, TFormatType>::base_classes_count> {});
            }
        }

        // =====================================================================================
        // =====================================================================================
        // =====================================================================================

        /**
         * Serializes derived type (BaseT) of parent type (OriginalT)
         * @tparam OriginalT
         * @tparam BaseT
         * @param valueView
         */
        template<typename OriginalT, typename DerivedT, FormatType TFormatType>
        static void serializeDerivedType(SerializableValueView<OriginalT, TFormatType>& valueView) noexcept
        {
            const auto* derivedTypeObj = dynamic_cast<const DerivedT*>(valueView.m_data);
            if(derivedTypeObj)
            {
                // converting OriginalT value view to DerivedT value view to pass into SerdeSpec
                SerializableValueView<DerivedT, TFormatType> tmpView { };
                valueView.template copyFormatPointers<DerivedT>(tmpView);
                tmpView.m_data = derivedTypeObj;

                // todo: setting new type of value in typeName section

                // trying to serialize as one of DerivedT`s derived types
                serializeDerivedTypes<DerivedT, TFormatType>(tmpView);

                // passing tmpView with DerivedT to SerdeSpec
                SerdeSpec<DerivedT, TFormatType>::serialize(tmpView);
            }
        }

        /**
         * Implementation of serializing derived types of T. Uses unpacking.
         * @tparam T
         * @tparam Indices
         * @param valueView
         */
        template<typename T, FormatType TFormatType, std::size_t... Indices>
        static void serializeDerivedTypesImpl(SerializableValueView<T, TFormatType>& valueView,
                                              std::index_sequence<Indices...>) noexcept
        {
            // unpacking variadic template
            (serializeDerivedType<T,
                    typename SerdeSpec<T, TFormatType>::template get_derived_type<Indices>,
                    TFormatType>(valueView), ...);
        }

        /**
         * Serializes all derived types of T.
         * @tparam T
         * @param valueView
         */
        template<typename T, FormatType TFormatType>
        static void serializeDerivedTypes(SerializableValueView<T, TFormatType>& valueView) noexcept
        {
            // serializing derived types only if information of them was provided
            if constexpr(Utils::isDerivedTypesProvided<T, TFormatType>())
            {
                serializeDerivedTypesImpl<T, TFormatType>
                        (valueView, std::make_index_sequence<SerdeSpec<T, TFormatType>::derived_classes_count> {});
            }
        }
    };

    // =========================================================================================
    // STANDARD SerdeSpec IMPLEMENTATIONS
    // =========================================================================================

    template<typename T, FormatType TFormatType>
    struct SerdeSpec<std::unique_ptr<T>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = SerdeSpec<T, TFormatType>::type_name;
        static inline constexpr bool is_pointer_type = true;
        using element_type = T;

        static void serialize(SerializableValueView<std::unique_ptr<T>, TFormatType>& valueView)
        {
            if(!*valueView.m_data)
            {
                // TODO: serialize nullptr
                return;
            }

            SerializableValueView<T, TFormatType> tmpView = { };
            valueView.template copyFormatPointers<T>(tmpView);
            tmpView.m_data = valueView.m_data->get();

            // todo: serializing values of T
            SerdeSpec<T, TFormatType>::serialize(tmpView);
        }

        static void deserialize(DeserializableValueView<std::unique_ptr<T>, TFormatType>& valueView)
        {

        }
    };
}

#endif //SUNGEARENGINE_NEWSERIALIZER_H
