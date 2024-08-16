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
    template<typename T>
    struct IDeserializableValueView
    {

    };

    template<typename T>
    struct JSONSerializableValueView;

    template<FormatType TFormatType>
    struct SerializableValueViewFormatPointers
    {
        static_assert(always_false<decltype(TFormatType)>::value,
                      "This format is not implemented. (For engine devs: please, implement this format)");

        // must be implemented
        void copyFormatPointers(SerializableValueViewFormatPointers<TFormatType>& to) const noexcept
        {

        }
    };

    template<>
    struct SerializableValueViewFormatPointers<FormatType::JSON>
    {
        void copyFormatPointers(SerializableValueViewFormatPointers<FormatType::JSON>& to) const noexcept
        {
            to.m_document = m_document;

            std::printf("called json copyFormatPointersImpl\n");
        }

    protected:
        rapidjson::Document* m_document { };
    };

    // DO NOT STORE ANYWHERE. DOES NOT OWN ANYTHING
    template<typename T>
    struct ISerializableValueView
    {
        friend struct Serializer;

        const T* m_data { };

        [[nodiscard]] FormatType getFormatType() const noexcept
        {
            return m_formatType;
        }

        template<typename T0>
        void copyFormatPointers(ISerializableValueView<T0>& to) noexcept
        {
            assert(to.getFormatType() == m_formatType &&
                   "SGCore::Serde::ISerializableValueView::copyFormatPointers: [to] format and [from] format types are not equal.");

            switch(to.getFormatType())
            {
                case FormatType::JSON:
                    reinterpret_cast<SerializableValueViewFormatPointers<FormatType::JSON>*>(this)->copyFormatPointers(
                            reinterpret_cast<SerializableValueViewFormatPointers<FormatType::JSON>&>(to));
                    break;
                case FormatType::BSON:
                    break;
                case FormatType::YAML:
                    break;
            }
        }

    protected:
        FormatType m_formatType = FormatType::JSON;
        Ref<ISerializableValueView<T>> m_parent;
    };

    template<typename T>
    struct JSONSerializableValueView final : public ISerializableValueView<T>,
                                             public SerializableValueViewFormatPointers<FormatType::JSON>
    {
        friend struct Serializer;

        JSONSerializableValueView()
        {
            this->m_formatType = FormatType::JSON;
        }
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
        static void serialize(ISerializableValueView<T>& valueView)
        {

        }

        /**
         * Dserializes only T type members.
         * @param valueView
         */
        static void deserialize(IDeserializableValueView<T>& valueView)
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
            switch (m_defaultFormatType)
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

        template<typename T>
        static void serializeMember(ISerializableValueView<T>& parent,
                                    const std::string& varName,
                                    const T& value) noexcept
        {

        }

        /**
         * Creates value view for serializable value depending on current format type
         * @tparam T
         * @param formatType
         * @return
         */
        template<typename T>
        static Ref<ISerializableValueView<T>> createSerializableValueView(FormatType formatType) noexcept
        {
            switch (formatType)
            {
                case FormatType::JSON:
                    return MakeRef<JSONSerializableValueView<T>>();
                case FormatType::BSON:
                    return nullptr;
                case FormatType::YAML:
                    return nullptr;
            }

            return nullptr;
        }

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
            auto valueView = MakeRef<JSONSerializableValueView<T>>();
            valueView->m_data = &value;
            valueView->m_document = &toDocument;

            // serializing value with attempt at dynamic casts to derived types
            serializeWithDynamicChecks<T, FormatType::JSON>(*valueView);

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
        static void serializeWithDynamicChecks(ISerializableValueView<T>& valueView)
        {
            if constexpr(SerdeSpec<T, TFormatType>::is_pointer_type) // serializing value using dynamic checks
            {
                // getting element_type that pointer contains
                using ptr_element_type = SerdeSpec<T, TFormatType>::element_type;

                // creating view that contains element_type object
                auto tmpView = Serializer::createSerializableValueView<ptr_element_type>(valueView.getFormatType());
                valueView.template copyFormatPointers<ptr_element_type>(*tmpView);
                tmpView->m_data = &(**valueView.m_data);

                // serializing base types
                serializeBaseTypes<ptr_element_type, TFormatType>(*tmpView);

                // serializing derived types
                serializeDerivedTypes<ptr_element_type, TFormatType>(*tmpView);

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
        static void serializeBaseType(ISerializableValueView<OriginalT>& valueView) noexcept
        {
            // converting OriginalT value view to BaseT value view to pass into SerdeSpec
            auto tmpView = Serializer::createSerializableValueView<BaseT>(valueView.getFormatType());
            valueView.template copyFormatPointers<BaseT>(*tmpView);
            tmpView->m_data = &(static_cast<const BaseT&>(*valueView.m_data));

            // serialize BaseT`s base types
            serializeBaseTypes<BaseT, TFormatType>(*tmpView);

            // passing tmpView with BaseT to SerdeSpec
            SerdeSpec<BaseT, TFormatType>::serialize(*tmpView);
        }

        /**
         * Implementation of serializing base types of T. Uses unpacking.
         * @tparam T
         * @tparam Indices
         * @param valueView
         */
        template<typename T, FormatType TFormatType, std::size_t... Indices>
        static void serializeBaseTypesImpl(ISerializableValueView<T>& valueView,
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
        static void serializeBaseTypes(ISerializableValueView<T>& valueView) noexcept
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
        static void serializeDerivedType(ISerializableValueView<OriginalT>& valueView) noexcept
        {
            const auto* derivedTypeObj = dynamic_cast<const DerivedT*>(valueView.m_data);
            if(derivedTypeObj)
            {
                // converting OriginalT value view to DerivedT value view to pass into SerdeSpec
                auto tmpView = Serializer::createSerializableValueView<DerivedT>(valueView.getFormatType());
                valueView.template copyFormatPointers<DerivedT>(*tmpView);
                tmpView->m_data = derivedTypeObj;

                // todo: setting new type of value in typeName section

                // trying to serialize as one of DerivedT`s derived types
                serializeDerivedTypes<DerivedT, TFormatType>(*tmpView);

                // passing tmpView with DerivedT to SerdeSpec
                SerdeSpec<DerivedT, TFormatType>::serialize(*tmpView);
            }
        }

        /**
         * Implementation of serializing derived types of T. Uses unpacking.
         * @tparam T
         * @tparam Indices
         * @param valueView
         */
        template<typename T, FormatType TFormatType, std::size_t... Indices>
        static void serializeDerivedTypesImpl(ISerializableValueView <T>& valueView,
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
        static void serializeDerivedTypes(ISerializableValueView<T>& valueView) noexcept
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

        static void serialize(ISerializableValueView<std::unique_ptr<T>>& valueView)
        {
            if(!*valueView.m_data)
            {
                // TODO: serialize nullptr
                return;
            }

            auto tmpView = Serializer::createSerializableValueView<T>(valueView.getFormatType());
            valueView.template copyFormatPointers<T>(*tmpView);
            tmpView->m_data = valueView.m_data->get();

            // todo: serializing values of T
            SerdeSpec<T, TFormatType>::serialize(*tmpView);
        }

        static void deserialize(IDeserializableValueView<std::unique_ptr<T>>& valueView)
        {

        }
    };
}

#endif //SUNGEARENGINE_NEWSERIALIZER_H
