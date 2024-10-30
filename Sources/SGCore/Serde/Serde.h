//
// Created by Ilya on 14.08.2024.
//

#ifndef SUNGEARENGINE_SERDE_H
#define SUNGEARENGINE_SERDE_H

#include <optional>
#include "SGCore/Utils/Assert.h"

#include "SGCore/Utils/TypeTraits.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Main/CoreGlobals.h"

#include "Common.h"

namespace SGCore::Serde
{
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

        using base_types_container = types_container<Cls...>;
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

        using derived_types_container = types_container<Cls...>;
    };

    // FORWARD DECLARATIONS =======================================================

    template<typename T, FormatType TFormatType>
    struct SerdeSpec;

    template<FormatType TFormatType>
    struct SerializableValueContainer;

    template<FormatType TFormatType>
    struct DeserializableValueContainer;

    template<typename T, FormatType TFormatType>
    struct DeserializableValueView;

    template<typename T, FormatType TFormatType>
    struct SerializableValueView;

    struct Serializer;

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
         * OPTIONAL (IF TYPE IS POINTER): type that pointer points to
         */
        using element_type = void;
        
        /**
         * Serializes only T type members. DO NOT IMPLEMENT IN SerdeSpec OF POINTER TYPES.
         * @param valueView
         */
        static void serialize(SerializableValueView<T, TFormatType>& valueView)
        {
        
        }
        
        /**
         * Deserializes only T type members. DO NOT IMPLEMENT IN SerdeSpec OF POINTER TYPES.
         * @param valueView
         */
        static void deserialize(DeserializableValueView<T, TFormatType>& valueView)
        {
        
        }

        /**
         * OPTIONAL (BUT NECESSARY IN IMPLEMENTATIONS OF SerdeSpec OF POINTER TYPES).
         * Allocates object with type that is derived type of T or allocates T object.
         * Possible uses: allocation of the ITexture2D object depending on the currently used graphics API.
         * @return
         */
        static int allocateObject()
        {

        }
        
        /**
         * OPTIONAL (IF TYPE IS POINTER): getting raw pointer to object
         * @param valueView
         */
        template<typename ValueViewT>
        static T* getObjectRawPointer(ValueViewT& valueView) noexcept
        {
        
        }
        
        /**
         * OPTIONAL (IF TYPE IS POINTER): setting raw pointer to object
         * @param valueView
         */
        template<typename ValueViewT, typename T0>
        static void setObjectRawPointer(ValueViewT& valueView, T0* pointer) noexcept
        {
        
        }
    };

    // ==============================================================================
    // ==============================================================================
    // ==============================================================================

    /**
     * Information about format type.
     * @tparam TFormatType - Format Type.
     */
    template<FormatType TFormatType>
    struct FormatInfo
    {
        static_assert(always_false_obj<TFormatType>::value, "Can not get information about this format type. FormatInfo specialization for this format type was not defined.");

        using member_iterator_t = void;
        using array_iterator_t = void;
    };

    // ==============================================================================
    // ==============================================================================
    // ==============================================================================
    
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
            if constexpr(requires { SerdeSpec<T, TFormatType>::base_classes_count; })
            {
                if constexpr(SerdeSpec<T, TFormatType>::base_classes_count > 0)
                {
                    if constexpr(requires { typename SerdeSpec<T, TFormatType>::template get_base_type<0>; })
                    {
                        return true;
                    }
                }
            }
            
            return false;
        }

        template<typename T, FormatType TFormatType>
        static consteval size_t getBaseTypesCount()
        {
            if constexpr(isBaseTypesProvided<T, TFormatType>())
            {
                return SerdeSpec<T, TFormatType>::base_classes_count;
            }
            else
            {
                return 0;
            }
        }
        
        /**
        * Is SerdeSpec of T provides derived types.
        * @tparam T
        * @return
        */
        template<typename T, FormatType TFormatType>
        static consteval bool isDerivedTypesProvided()
        {
            if constexpr(requires { SerdeSpec<T, TFormatType>::derived_classes_count; })
            {
                if constexpr(SerdeSpec<T, TFormatType>::derived_classes_count > 0)
                {
                    if constexpr(requires { typename SerdeSpec<T, TFormatType>::template get_derived_type<0>; })
                    {
                        return true;
                    }
                }
            }
            
            return false;
        }

        template<typename T, FormatType TFormatType>
        static consteval size_t getDerivedTypesCount()
        {
            if constexpr(isDerivedTypesProvided<T, TFormatType>())
            {
                return SerdeSpec<T, TFormatType>::derived_classes_count;
            }
            else
            {
                return 0;
            }
        }
    };
    
    // ================================================================
    // ================================================================
    // ================================================================

    /**
     * EXAMPLE STRUCT.
     * @tparam TFormatType
     */
    template<FormatType TFormatType>
    class SerializerImpl
    {
        friend struct Serializer;

        static_assert(always_false_obj<TFormatType>::value, "Serde for this type was not implemented.");

        /**
         * Converts object to SOME format
         * @tparam T - Serializable type.
         * @param toDocument
         * @param value
         * @return
         */
        template<typename... SharedDataT, typename T>
        static std::string to(const T& value,
                              SharedDataT&&... sharedData) noexcept
        {
            return "";
        }

        /**
         * Converts SOME FORMAT document to object.
         * @tparam T
         * @param toDocument
         * @param value
         * @return
         */
        template<typename T, typename... SharedDataT>
        static void from(const std::string& formattedText,
                         std::string& outputLog,
                         T& outValue,
                         SharedDataT&&... sharedData) noexcept
        {

        }
    };

    // including forward decl of serializers
    #include "Implementations/JSON/SerializerForward.h"

    // ==============================================================================
    // ==============================================================================
    // ==============================================================================

    struct Serializer
    {
        template<typename T, FormatType TFormatType>
        friend struct SerializableValueView;
        
        template<FormatType TFormatType>
        friend struct SerializableValueContainer;
        
        template<FormatType TFormatType>
        friend struct DeserializableValueContainer;

        template<FormatType TFormatType>
        friend class SerializerImpl;
        
        /**
         * Converts object to some format (JSON, BSON, YAML, etc)
         * @tparam T - Serializable type.
         * @param value
         * @param formatType
         * @return
         */
        template<typename... SharedDataT, typename T>
        static std::string toFormat(FormatType formatType, T& value, SharedDataT&&... sharedData) noexcept
        {
            switch (formatType)
            {
                case FormatType::JSON:
                {
                    return SerializerImpl<FormatType::JSON>::to(value, std::forward<SharedDataT>(sharedData)...);
                }
                case FormatType::BSON:
                    break;
                case FormatType::YAML:
                    break;
            }
            
            return "";
        }

        /**
         * Converts object to some format (JSON, BSON, YAML, etc)
         * @tparam T - Serializable type.
         * @param value
         * @param formatType
         * @return
         */
        template<typename T>
        static std::string toFormat(T& value, FormatType formatType = m_defaultFormatType) noexcept
        {
            return toFormat(formatType, value);
        }
        
        /**
         * Deserializes object from some format (JSON, BSON, YAML, etc)
         * @tparam T
         * @param value
         * @param formatType
         * @return
         */
        template<typename... SharedDataT, typename T>
        static void fromFormat(const std::string& formattedText,
                               T& outValue,
                               FormatType formatType,
                               std::string& outputLog,
                               SharedDataT&&... sharedData) noexcept
        {
            switch (formatType)
            {
                case FormatType::JSON:
                {
                    SerializerImpl<FormatType::JSON>::from(formattedText, outputLog, outValue, std::forward<SharedDataT>(sharedData)...);

                    break;
                }
                case FormatType::BSON:
                    break;
                case FormatType::YAML:
                    break;
            }
        }

        /**
         * Deserializes object from some format (JSON, BSON, YAML, etc)
         * @tparam T
         * @param value
         * @param formatType
         * @return
         */
        template<typename T>
        static void fromFormat(const std::string& formattedText,
                               T& outValue,
                               std::string& outputLog,
                               FormatType formatType = m_defaultFormatType) noexcept
        {
            fromFormat(formattedText, outValue, formatType, outputLog);
        }

        template<typename T, FormatType TFormatType, typename... SharedDataT>
        static void invokeSerdeSpecSerialize(SerializableValueView<T, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
        {
            // if serialize function of SerdeSpec is template
            if constexpr(requires { SerdeSpec<T, TFormatType>::template serialize<SharedDataT...>; })
            {
                // if we can call SerdeSpec<T, TFormatType>::serialize with passing sharedData
                if constexpr(std::is_invocable_v<decltype(SerdeSpec<T, TFormatType>::template serialize<SharedDataT...>),
                        decltype(valueView),
                        SharedDataT...>)
                {
                    SerdeSpec<T, TFormatType>::serialize(valueView, std::forward<SharedDataT>(sharedData)...);
                }
                else
                {
                    // else if we can not call SerdeSpec<T, TFormatType>::serialize than passing only valueView without sharedData
                    SerdeSpec<T, TFormatType>::serialize(valueView);
                }
            }
            else
            {
                // if we can call SerdeSpec<T, TFormatType>::serialize with passing sharedData
                if constexpr(std::is_invocable_v<decltype(SerdeSpec<T, TFormatType>::serialize),
                        decltype(valueView),
                        SharedDataT...>)
                {
                    SerdeSpec<T, TFormatType>::serialize(valueView, std::forward<SharedDataT>(sharedData)...);
                }
                else
                {
                    // else if we can not call SerdeSpec<T, TFormatType>::serialize than passing only valueView without sharedData
                    SerdeSpec<T, TFormatType>::serialize(valueView);
                }
            }
        }

        template<typename T, FormatType TFormatType, typename... SharedDataT>
        static void invokeSerdeSpecDeserialize(DeserializableValueView<T, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
        {
            // if serialize function of SerdeSpec is template
            if constexpr(requires { SerdeSpec<T, TFormatType>::template deserialize<SharedDataT...>; })
            {
                // if we can call SerdeSpec<T, TFormatType>::serialize with passing sharedData
                if constexpr(std::is_invocable_v<decltype(SerdeSpec<T, TFormatType>::template deserialize<SharedDataT...>),
                        decltype(valueView),
                        SharedDataT...>)
                {
                    SerdeSpec<T, TFormatType>::deserialize(valueView, std::forward<SharedDataT>(sharedData)...);
                }
                else
                {
                    // else if we can not call SerdeSpec<T, TFormatType>::serialize than passing only valueView without sharedData
                    SerdeSpec<T, TFormatType>::deserialize(valueView);
                }
            }
            else
            {
                // if we can call SerdeSpec<T, TFormatType>::serialize with passing sharedData
                if constexpr(std::is_invocable_v<decltype(SerdeSpec<T, TFormatType>::deserialize),
                        decltype(valueView),
                        SharedDataT...>)
                {
                    SerdeSpec<T, TFormatType>::deserialize(valueView, std::forward<SharedDataT>(sharedData)...);
                }
                else
                {
                    // else if we can not call SerdeSpec<T, TFormatType>::serialize than passing only valueView without sharedData
                    SerdeSpec<T, TFormatType>::deserialize(valueView);
                }
            }
        }
    
    private:
        static inline FormatType m_defaultFormatType = SGCore::Serde::FormatType::JSON;

        /// Collects all base types of \p FromT type and returns \p std::tuple with all base types from very base type to \p FromT type
        template<typename FromT, FormatType TFormatType>
        struct collect_all_base_types
        {
        private:
            template<typename CurrentT, size_t... Indices>
            consteval static auto impl(std::index_sequence<Indices...>)
            {
                using out_type = types_container_cat_t<
                        typename SerdeSpec<CurrentT, TFormatType>::base_types_container,
                        typename collect_all_base_types<typename SerdeSpec<CurrentT, TFormatType>::template get_base_type<Indices>, TFormatType>::not_reversed_type...>;

                return out_type { };
            }

            consteval static auto get()
            {
                if constexpr(Utils::isBaseTypesProvided<FromT, TFormatType>())
                {
                    return decltype(impl<FromT>(std::make_index_sequence<Utils::getBaseTypesCount<FromT, TFormatType>()> {})) { };
                }
                else
                {
                    return types_container<> { };
                }
            }

        public:
            using not_reversed_type = std::remove_cvref_t<decltype(get())>;
            using type = reverse_types_container_t<not_reversed_type>;
        };
        
        /**
         * Serializes value with attempt at dynamic casts to derived types.\n
         * If all attempts of casting value to derived types are failed than serializing only T type members and members of all base types.
         * @tparam T - Serializable type.
         * @param valueView
         */
        template<typename T,
                 FormatType TFormatType,
                 typename... SharedDataT>
        static void serializeWithDynamicChecksImpl(SerializableValueView<T, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            if constexpr(SerdeSpec<T, TFormatType>::is_pointer_type) // serializing value using dynamic checks
            {
                // if value of passed pointer equals to nullptr
                if(!SerdeSpec<T, TFormatType>::getObjectRawPointer(valueView))
                {
                    // setting value as null in output container
                    valueView.getValueContainer().setAsNull();
                    return;
                }

                // getting element_type that pointer contains
                using ptr_element_type = SerdeSpec<T, TFormatType>::element_type;
                
                // creating view that contains element_type object
                SerializableValueView<ptr_element_type, TFormatType> tmpView { };
                tmpView.getValueContainer() = valueView.getValueContainer();
                tmpView.m_version = valueView.m_version;
                tmpView.m_data = SerdeSpec<T, TFormatType>::getObjectRawPointer(valueView);

                // trying to serialize as one of derived types o ptr_element_type
                tryToSerializeAsDerivedType<ptr_element_type, TFormatType>(tmpView, std::forward<SharedDataT>(sharedData)...);
            }
            else
            {
                // getting all base types from very base type in inheritance tree to very derived type
                using base_types = typename collect_all_base_types<T, TFormatType>::type;

                // serialize without dynamic checks (static serialization)

                // serializing all base types from very base type in inheritance tree to very derived type
                serializeBaseTypes<base_types>(valueView, std::make_index_sequence<base_types::types_count>(),
                                               std::forward<SharedDataT>(sharedData)...);

                // serializing only T type members
                invokeSerdeSpecSerialize(valueView, std::forward<SharedDataT>(sharedData)...);

                // setting new type name
                valueView.getValueContainer().setTypeName(SerdeSpec<T, TFormatType>::type_name);
            }
        }

        template<typename T,
                 FormatType TFormatType,
                 typename... SharedDataT>
        static void serializeWithDynamicChecks(SerializableValueView<T, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            serializeWithDynamicChecksImpl<T, TFormatType, SharedDataT...>(valueView, std::forward<SharedDataT>(sharedData)...);
        }
        
        /**
         * Deserializes value with attempt at dynamic casts to derived types.\n
         * If all attempts of casting value to derived types are failed than serializing only T type members and members of all base types.\n
         * If one of derived types is matches to typeName than it will be deserialized.
         * @tparam T
         * @param valueView
         */
        template<typename T, FormatType TFormatType, typename... SharedDataT>
        static void deserializeWithDynamicChecksImpl(DeserializableValueView<T, TFormatType>& valueView,
                                                     SharedDataT&&... sharedData)
        {
            if constexpr(SerdeSpec<T, TFormatType>::is_pointer_type) // deserializing value using dynamic checks
            {
                // if value of deserializable type equals to null then just returning
                if(valueView.getValueContainer().isNull())
                {
                    return;
                }

                // getting element_type that pointer contains
                using ptr_element_type = SerdeSpec<T, TFormatType>::element_type;
                
                // creating view that contains element_type object
                DeserializableValueView<ptr_element_type, TFormatType> tmpView { };
                tmpView.getValueContainer() = valueView.getValueContainer();
                tmpView.m_version = valueView.m_version;

                // trying to deserialize T as one of its derived types (this function will allocate object)
                deserializeAsOneOfDerivedTypes<ptr_element_type, TFormatType>(tmpView, std::forward<SharedDataT>(sharedData)...);
                
                // if one of derived types T was deserialized
                if(tmpView.m_data)
                {
                    // setting new raw pointer to value view
                    SerdeSpec<T, TFormatType>::setObjectRawPointer(valueView, tmpView.m_data);
                }
                else
                {
                    // collecting all base types of ptr_element_type in very base type to DerivedT order
                    using base_types = typename collect_all_base_types<ptr_element_type, TFormatType>::type;

                    if constexpr(!std::is_abstract_v<ptr_element_type>)
                    {
                        // allocating object. all SerdeSpec for pointer types must provide this function
                        *valueView.m_data = SerdeSpec<T, TFormatType>::allocateObject();
                    }
                    else
                    {
                        if constexpr(requires { SerdeSpec<ptr_element_type, TFormatType>::allocateObject; })
                        {
                            auto* object = SerdeSpec<ptr_element_type, TFormatType>::allocateObject();
                            // setting raw pointer ('object') to 'valueView'
                            SerdeSpec<T, TFormatType>::setObjectRawPointer(valueView, object);
                        }
                        else
                        {
                            // we have no derived types of 'ptr_element_type' but 'ptr_element_type' is abstract type
                            if constexpr(!Utils::isDerivedTypesProvided<ptr_element_type, TFormatType>())
                            {
                                static_assert(always_false<ptr_element_type>::value,
                                        "Can not allocate object of abstract type (using new expression or SerdeSpec<AbstractType, TFormatType>::allocateObject().");
                            }

                            // NOT static_assert BECAUSE WE CAN NOT DETERMINE IF OBJECT WAS DESERIALIZED AS DERIVED TYPE IN COMPILE-TIME
                            //
                            // Theoretically, this case is possible only with initially incorrectly serialized data, which is now being deserialized.
                            // An abstract type cannot be allocated in a program, therefore, only a non-abstract type
                            // inheriting the current abstract type can be allocated, and therefore,
                            // this non-abstract type must always be serialized and deserialized.
                            //
                            // Also, such behavior is possible when forgetting to specify a non-abstract type as a type inheriting
                            // the current abstract type in the SerdeSpec implementation of the current abstract type.
                            // Please, make sure that you specify all derived types in BaseTypes of SerdeSpec<ptr_element_type (abstract type), TFormatType>
                            const std::string assertMsg = fmt::format("Can not allocate object of abstract type (using new expression or SerdeSpec<{}, TFormatType>::allocateObject(). Abstract type is: {}",
                                                                      typeid(ptr_element_type).name(),
                                                                      typeid(ptr_element_type).name());

                            SG_ASSERT(false, assertMsg.data());
                        }
                    }

                    // assigning allocated pointer
                    tmpView.m_data = SerdeSpec<T, TFormatType>::getObjectRawPointer(valueView);

                    // then we are deserializing all base types
                    // deserializing base types of ptr_element_type
                    deserializeBaseTypes<base_types>(tmpView, std::forward<SharedDataT>(sharedData)...);

                    // finally we are calling deserialization of ptr_element_type type members
                    invokeSerdeSpecDeserialize(tmpView, std::forward<SharedDataT>(sharedData)...);
                }
            }
            else
            {

                if(valueView.getValueContainer().m_typeName != SerdeSpec<T, TFormatType>::type_name)
                {
                    *valueView.getValueContainer().m_outputLog +=
                            "Can not deserialize value with type '" + valueView.getValueContainer().m_typeName +
                            "' using T as '" + SerdeSpec<T, TFormatType>::type_name + "'.\n";

                    return;
                }

                // deserialize without dynamic checks (static deserialization)

                // collecting all base types of T in very base type to DerivedT order
                using base_types = typename collect_all_base_types<T, TFormatType>::type;

                // deserializing base types
                deserializeBaseTypes<base_types>(valueView, std::forward<SharedDataT>(sharedData)...);

                // deserializing only T type members
                invokeSerdeSpecDeserialize(valueView, std::forward<SharedDataT>(sharedData)...);
            }
        }
        
        template<typename T, FormatType TFormatType, typename... SharedDataT>
        static void deserializeWithDynamicChecks(DeserializableValueView<T, TFormatType>& valueView, SharedDataT&&... sharedData)
        {
            deserializeWithDynamicChecksImpl<T, TFormatType>(valueView, std::forward<SharedDataT>(sharedData)...);
        }
        
        // =====================================================================================
        // =====================================================================================
        // =====================================================================================
        
        /**
         * Tries to find correct derived type
         * @tparam OriginalT
         * @tparam BaseT
         * @param valueView
         */
        template<typename OriginalT, FormatType TFormatType, size_t CurrentDerivedIdx, typename... SharedDataT>
        static void trySerializeAsDerivedType(SerializableValueView<OriginalT, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
        {
            if constexpr(Utils::isDerivedTypesProvided<OriginalT, TFormatType>())
            {
                if constexpr(CurrentDerivedIdx < SerdeSpec<OriginalT, TFormatType>::derived_classes_count)
                {
                    using CurrentDerivedT = typename SerdeSpec<OriginalT, TFormatType>::template get_derived_type<CurrentDerivedIdx>;

                    const CurrentDerivedT* derivedTypeObj = nullptr;
                    if constexpr(std::is_polymorphic_v<OriginalT>)
                    {
                        derivedTypeObj = dynamic_cast<const CurrentDerivedT*>(valueView.m_data);
                    }

                    if(derivedTypeObj)
                    {

                        SerializableValueView<CurrentDerivedT, TFormatType> tmpView {};
                        tmpView.getValueContainer() = valueView.getValueContainer();
                        tmpView.m_version = valueView.m_version;
                        tmpView.m_data = derivedTypeObj;

                        trySerializeAsDerivedType<CurrentDerivedT, TFormatType, 0>(tmpView,
                                                                                   std::forward<SharedDataT>(sharedData)...
                        );
                    }
                    else
                    {
                        // continue to search needed derived type in derived types of OriginalT types
                        trySerializeAsDerivedType<OriginalT, TFormatType,
                                CurrentDerivedIdx + 1>(valueView,
                                                       std::forward<SharedDataT>(
                                                               sharedData
                                                       )...
                        );
                    }
                }
            }

            if constexpr(Utils::isDerivedTypesProvided<OriginalT, TFormatType>())
            {
                // WE ARE IN THE END OF INHERITANCE TREE FOR CURRENT OBJECT. DESERIALIZING ALL BASE TYPES OF OriginalT FROM VERY BASE TYPE TO OriginalT type
                // if we did not find correct derived type on current level then trying to serialize last correct derived type (last is: 'OriginalT')
                // collecting all base types of OriginalT in very base type to DerivedT order
                if(CurrentDerivedIdx == SerdeSpec<OriginalT, TFormatType>::derived_classes_count)
                {
                    serializeObjectStraitDownByTree(valueView, std::forward<SharedDataT>(sharedData)...);
                }
            }
            else
            {
                // WE ARE IN THE END OF INHERITANCE TREE FOR CURRENT OBJECT. DESERIALIZING ALL BASE TYPES OF OriginalT FROM VERY BASE TYPE TO OriginalT type
                // if we did not find correct derived type on current level then trying to serialize last correct derived type (last is: 'OriginalT')
                // collecting all base types of OriginalT in very base type to DerivedT order

                serializeObjectStraitDownByTree(valueView, std::forward<SharedDataT>(sharedData)...);
            }
        }

        template<typename BaseType, typename OriginalT, FormatType TFormatType, typename... SharedDataT>
        static void serializeBaseType(SerializableValueView<OriginalT, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
        {
            // Casting object to base type
            const auto* baseObj = static_cast<const BaseType*>(valueView.m_data);

            // Creating container for base type
            SerializableValueView<BaseType, TFormatType> tmpView {};
            tmpView.getValueContainer() = valueView.getValueContainer();
            tmpView.m_version = valueView.m_version;
            tmpView.m_data = baseObj;

            invokeSerdeSpecSerialize(tmpView, std::forward<SharedDataT>(sharedData)...);
        }

        /**
         * Serializes all types in \p BaseTypesCont from very base type to very derived type.
         * @tparam BaseTypesCont
         * @tparam OriginalT
         * @tparam TFormatType
         * @tparam SharedDataT
         * @tparam BaseTypesIndices
         * @param valueView
         * @param sharedData
         */
        template<types_container_t BaseTypesCont, typename OriginalT, FormatType TFormatType, size_t... BaseTypesIndices, typename... SharedDataT>
        static void serializeBaseTypes(SerializableValueView<OriginalT, TFormatType>& valueView, std::index_sequence<BaseTypesIndices...>, SharedDataT&&... sharedData) noexcept
        {
            (serializeBaseType<typename BaseTypesCont::template get_type<BaseTypesIndices>>(valueView, std::forward<SharedDataT>(sharedData)...), ...);
        }

        /**
         * Serializes all types in \p BaseTypesCont from very base type to very derived type.
         * @tparam TypeToSerialize
         * @tparam TFormatType
         * @tparam SharedDataT
         * @param valueView
         * @param sharedData
         */
        template<typename TypeToSerialize, FormatType TFormatType, typename... SharedDataT>
        static void serializeObjectStraitDownByTree(SerializableValueView<TypeToSerialize, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
        {
            using base_types = typename collect_all_base_types<TypeToSerialize, TFormatType>::type;

            std::cout << "serializing type...:: " << typeid(TypeToSerialize).name() << std::endl;

            // calling serialize all base types using 'base_types'
            serializeBaseTypes<base_types>(valueView, std::make_index_sequence<base_types::types_count>(),
                                           std::forward<SharedDataT>(sharedData)...);

            // calling serialize 'found_correct_derived_type'
            invokeSerdeSpecSerialize(valueView, std::forward<SharedDataT>(sharedData)...);

            // setting new type name
            valueView.getValueContainer().setTypeName(SerdeSpec<TypeToSerialize, TFormatType>::type_name);
        }
        
        /**
         * Serializes all derived types of T.
         * @tparam T - Serializable type.
         * @param valueView
         */
        template<typename T, FormatType TFormatType, typename... SharedDataT>
        static void tryToSerializeAsDerivedType(SerializableValueView<T, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
        {
            // serializing derived types only if information of them was provided and count of derived types > 0
            if constexpr(Utils::isDerivedTypesProvided<T, TFormatType>())
            {
                trySerializeAsDerivedType<T, TFormatType, 0>(valueView, std::forward<SharedDataT>(sharedData)...);
            }
            else
            {
                // collecting all base types of T
                using base_types = typename collect_all_base_types<T, TFormatType>::type;

                std::cout << "serializing type...:: " << typeid(T).name() << std::endl;

                // calling serialize all base types using 'base_types'
                serializeBaseTypes<base_types>(valueView, std::make_index_sequence<base_types::types_count>(),
                                               std::forward<SharedDataT>(sharedData)...);

                // calling serialization of T
                invokeSerdeSpecSerialize(valueView, std::forward<SharedDataT>(sharedData)...);
            }
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
        template<typename OriginalT, typename BaseT, FormatType TFormatType, typename... SharedDataT>
        static void deserializeBaseType(DeserializableValueView<OriginalT, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
        {
            // converting OriginalT value view to BaseT value view to pass into SerdeSpec
            DeserializableValueView<BaseT, TFormatType> tmpView { };
            tmpView.getValueContainer() = valueView.getValueContainer();
            tmpView.m_version = valueView.m_version;
            tmpView.m_data = &(static_cast<BaseT&>(*valueView.m_data));
            
            // serialize BaseT`s base types
            // NOTE: DO NOT DO THIS. IN NEW IMPLEMENTATION WE HAVE STRICT FROM VERY BASE TO DERIVED TYPE DESERIALIZATION.
            // deserializeBaseTypes(tmpView, std::forward<SharedDataT>(sharedData)...);
            
            // passing tmpView with BaseT to SerdeSpec
            invokeSerdeSpecDeserialize(tmpView, std::forward<SharedDataT>(sharedData)...);
        }
        
        /**
         * Implementation of serializing base types of T. Uses unpacking.
         * @tparam T
         * @tparam Indices
         * @param valueView
         */
        template<typename T, FormatType TFormatType, types_container_t CollectedBaseTypes, typename... SharedDataT, std::size_t... Indices>
        static void deserializeBaseTypesImpl(DeserializableValueView<T, TFormatType>& valueView,
                                             std::index_sequence<Indices...>,
                                             SharedDataT&&... sharedData) noexcept
        {
            // unpacking variadic template
            (deserializeBaseType<T,
                    typename CollectedBaseTypes::template get_type<Indices>,
                    TFormatType>(valueView, std::forward<SharedDataT>(sharedData)...), ...);
        }
        
        /**
         * Deserializes all base types of T.
         * @tparam T
         * @param valueView
         */
        template<types_container_t CollectedBaseTypes, typename T, FormatType TFormatType, typename... SharedDataT>
        static void deserializeBaseTypes(DeserializableValueView<T, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
        {
            std::cout << "deserializeBaseTypes " << std::string(GENERATOR_PRETTY_FUNCTION) << std::endl;

            deserializeBaseTypesImpl<T, TFormatType, CollectedBaseTypes>
                    (valueView, std::make_index_sequence<CollectedBaseTypes::types_count> {}, std::forward<SharedDataT>(sharedData)...);
        }
        
        // =========================================================================

        /**
         * Tries to deserialize value in OriginalT value view as DerivedT.
         * @tparam OriginalT
         * @tparam DerivedT
         * @tparam TFormatType
         * @param valueView
         */
        template<typename OriginalT, typename DerivedT, FormatType TFormatType, typename... SharedDataT>
        static void deserializeAsDerivedType(DeserializableValueView<OriginalT, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
        {
            // if pointer was already allocated then we are thinking that correct derived object was already allocated
            if(valueView.m_data)
            {
                return;
            }

            // creating temporary view that contains pointer to DerivedT
            DeserializableValueView<DerivedT, TFormatType> tmpView { };
            tmpView.getValueContainer() = valueView.getValueContainer();
            tmpView.m_version = valueView.m_version;
            
            // typeNames are equal. DerivedT is suitable
            if(valueView.getValueContainer().m_typeName == SerdeSpec<DerivedT, TFormatType>::type_name)
            {
                // allocating object of DerivedT
                DerivedT* derivedObject { };
                if constexpr(!std::is_abstract_v<DerivedT>)
                {
                    derivedObject = new DerivedT();
                }
                else
                {
                    if constexpr(requires { SerdeSpec<DerivedT, TFormatType>::allocateObject; })
                    {
                        derivedObject = SerdeSpec<DerivedT, TFormatType>::allocateObject();
                    }
                    else
                    {
                        static_assert(always_false<DerivedT>::value, "Can not allocate object of abstract type (using new expression or SerdeSpec<AbstractType, TFormatType>::allocateObject().");
                    }
                }

                tmpView.m_data = derivedObject;

                // collecting all base types of DerivedT in very base type to DerivedT order
                using base_types = typename collect_all_base_types<DerivedT, TFormatType>::type;
                
                // deserializing base types of DerivedT
                deserializeBaseTypes<base_types>(tmpView, std::forward<SharedDataT>(sharedData)...);
                
                // deserializing members of DerivedT
                invokeSerdeSpecDeserialize(tmpView, std::forward<SharedDataT>(sharedData)...);
                
                // assigning allocated pointer to original valueView
                valueView.m_data = derivedObject;

                std::cout << "deserializing as derived " << std::string(GENERATOR_PRETTY_FUNCTION) << std::endl;
                
                return;
            }
            
            // if DerivedT is not suitable than continue to search
            deserializeAsOneOfDerivedTypes(tmpView, std::forward<SharedDataT>(sharedData)...);
            
            // assigning allocated pointer to original valueView
            valueView.m_data = tmpView.m_data;
        }

        template<typename T,
                 FormatType TFormatType,
                 typename... SharedDataT,
                 std::size_t... BuiltinDerivedTypesIndices>
        static void deserializeAsOneOfDerivedTypesImpl(DeserializableValueView<T, TFormatType>& valueView,
                                                       std::index_sequence<BuiltinDerivedTypesIndices...>,
                                                       SharedDataT&&... sharedData) noexcept
        {
            // unpacking variadic template
            (deserializeAsDerivedType<T,
                    typename SerdeSpec<T, TFormatType>::template get_derived_type<BuiltinDerivedTypesIndices>,
                    TFormatType>(valueView, std::forward<SharedDataT>(sharedData)...), ...);
        }

        /**
         * Tries to deserialize document value in valueView as one of derived types of T.
         * @tparam T
         * @tparam TFormatType
         * @param valueView
         */
        template<typename T, FormatType TFormatType, typename... SharedDataT>
        static void deserializeAsOneOfDerivedTypes(DeserializableValueView<T, TFormatType>& valueView, SharedDataT&&... sharedData) noexcept
        {
            // deserializing derived types only if information of them was provided and count of derived types > 0
            if constexpr(Utils::isDerivedTypesProvided<T, TFormatType>())
            {
                deserializeAsOneOfDerivedTypesImpl<T, TFormatType>
                        (valueView, std::make_index_sequence<SerdeSpec<T, TFormatType>::derived_classes_count> { }, std::forward<SharedDataT>(sharedData)...);

                return;
            }
        }
    };

    // including implementation of serializers
    #include "Implementations/JSON/SerializerImpl.h"

    /**
     * Container that contains pointers to object format specific types.\n
     * You must implement all next functions and members in your specialization of this struct.\n
     * DO NOT STORE ANYWHERE. DOES NOT OWN ANYTHING.
     * EXAMPLE STRUCT.
     * @tparam T
     * @tparam TFormatType
     */
    template<FormatType TFormatType>
    struct DeserializableValueContainer
    {
        template<FormatType>
        friend struct SerializerImpl;

        friend struct Serializer;

        template<typename T0, FormatType TFormatType0>
        friend struct DeserializableValueView;

        template<typename T, typename... SharedDataT>
        std::optional<T> getMember(const std::string& memberName, SharedDataT&&... sharedData) noexcept
        {

        }

        /**
         * Getting this container value as array.
         * @param f
         */
        template<typename T, typename... SharedDataT>
        [[nodiscard]] std::vector<T> getAsArray(SharedDataT&&... sharedData) noexcept
        {

        }

        /**
        * Getting this container value as int64.
        * @param f
        */
        [[nodiscard]] std::int64_t getAsInt64() const noexcept
        {

        }

        /**
        * Getting this container value as float.
        * @param f
        */
        [[nodiscard]] float getAsFloat() const noexcept
        {

        }

        /**
        * Getting this container value as string.
        * @param f
        */
        template<typename CharT>
        [[nodiscard]] std::basic_string<CharT> getAsString() const noexcept
        {

        }

        /**
        * Getting this container value as bool.
        * @param f
        */
        [[nodiscard]] bool getAsBool() const noexcept
        {

        }

        /**
         * Getting first iterator of members container.
         * @return First iterator of members container (begin).
         */
        [[nodiscard]] int memberBegin() const noexcept
        {

        }

        /**
         * Getting last iterator of members container.
         * @return Last iterator of members container (end).
         */
        [[nodiscard]] int memberEnd() const noexcept
        {

        }

        /**
        * Getting first iterator of array container.
        * @return First iterator of array container (begin).
        */
        [[nodiscard]] int begin() const noexcept
        {

        }

        /**
         * Getting last iterator of array container.
         * @return Last iterator of array container (end).
         */
        [[nodiscard]] int end() const noexcept
        {

        }

        /**
         * Getting member by iterator.
         * @tparam T - Type of member.
         * @param iterator - Member iterator.
         * @return Value of member.
         */
        template<typename T, typename... SharedDataT>
        T getMember(const int& iterator, SharedDataT&&... sharedData) noexcept
        {

        }

        /**
         * Getting element in array by iterator.
         * @tparam T - Type of member.
         * @param iterator - Array iterator.
         * @return Value of element.
         */
        template<typename T, typename... SharedDataT>
        T getMember(const float& iterator, SharedDataT&&... sharedData) noexcept
        {

        }

        /**
         * Getting name of member by iterator.
         * @param memberIterator
         * @return
         */
        [[nodiscard]] std::string getMemberName(const int& memberIterator) const noexcept
        {

        }

        [[nodiscard]] bool isNull() const noexcept
        {

        }

    private:
        DeserializableValueContainer<TFormatType>* m_parent { };
        std::string* m_outputLog { };
    };

    /**
     * Container that contains pointers to object format specific types.\n
     * You must implement all next functions and members in your specialization of this struct.\n
     * DO NOT STORE ANYWHERE. DOES NOT OWN ANYTHING.
     * EXAMPLE STRUCT.
     * @tparam T
     * @tparam TFormatType
     */
    template<FormatType TFormatType>
    struct SerializableValueContainer
    {
        template<FormatType>
        friend struct SerializerImpl;

        friend struct Serializer;

        template<typename T0, FormatType TFormatType0>
        friend struct SerializableValueView;

        /**
         * Add member to this container.\n
         * If member with that name is already exists than value of this member will be changed.
         * @tparam T
         * @param name
         * @param value
         */
        template<typename... SharedDataT, typename T>
        void addMember(const std::string& name, const T& value, SharedDataT&&... sharedData) noexcept
        {

        }

        /**
         * Add value in this container if it is array.
         * @tparam T
         * @param name
         * @param value
         */
        template<typename... SharedDataT, typename T>
        void pushBack(const T& value, SharedDataT&&... sharedData) noexcept
        {

        }

        /**
         * Setting this container value as null.
         * @param f
         */
        void setAsNull() noexcept
        {

        }

        /**
         * Setting this container value as float.
         * @param f
         */
        void setAsFloat(const float& f) noexcept
        {

        }

        /**
         * Setting this container value as int64.
         * @param f
         */
        void setAsInt64(const std::int64_t& i) noexcept
        {

        }

        /**
         * Setting this container value as string.
         * @param str
         */
        template<typename CharT>
        void setAsString(const std::basic_string<CharT>& str) noexcept
        {

        }

        /**
         * Setting this container value as array.\n
         * After calling this function you can call function pushBack.
         * @param f
         */
        void setAsArray() noexcept
        {

        }

        /**
         * Setting this container value as bool.
         * @param str
         */
        void setAsBool(bool b) noexcept
        {

        }

        /**
         * Setter for type name of value.
         * @param typeName
         */
        void setTypeName(const std::string& typeName) noexcept
        {

        }

        /**
         * Getter for type name of value.
         * @param typeName
         */
        [[nodiscard]] std::string getTypeName(const std::string& typeName) const noexcept
        {

        }

        /**
         * Returning parent container.
         * @return
         */
        auto& getParent() noexcept
        {
            return *m_parent;
        }

    private:
        SerializableValueContainer<TFormatType>* m_parent { };
    };

    // ==============================================================================
    // ==============================================================================
    // ==============================================================================

    /**
     * View of value with format type`s specific members. Contains serializable value info container.\n
     * You must implement all next functions and members in your specialization of this struct.\n
     * DO NOT STORE ANYWHERE. DOES NOT OWN ANYTHING.
     * @tparam T
     * @tparam TFormatType
     */
    template<typename T, FormatType TFormatType>
    struct SerializableValueView
    {
        template<FormatType>
        friend struct SerializerImpl;

        friend struct Serializer;

        // making all SerializableValueView as friends
        template<typename T0, FormatType TFormatType0>
        friend struct SerializableValueView;

        static inline constexpr FormatType format_type = TFormatType;

        const T* m_data { };

        SerializableValueContainer<TFormatType>& getValueContainer() noexcept
        {
            return m_valueContainer;
        }

    private:
        std::string m_version;

        SerializableValueContainer<TFormatType> m_valueContainer { };
    };

    /**
     * View of value with format type`s specific members. Contains deserializable value info container.\n
     * You must implement all next functions and members in your specialization of this struct.\n
     * DO NOT STORE ANYWHERE. DOES NOT OWN ANYTHING.
     * @tparam T
     * @tparam TFormatType
     */
    template<typename T, FormatType TFormatType>
    struct DeserializableValueView
    {
        template<FormatType>
        friend struct SerializerImpl;

        friend struct Serializer;

        // making all SerializableValueView as friends
        template<typename T0, FormatType TFormatType0>
        friend struct DeserializableValueView;

        static inline constexpr FormatType format_type = TFormatType;

        T* m_data { };

        DeserializableValueContainer<TFormatType>& getValueContainer() noexcept
        {
            return m_valueContainer;
        }

    private:
        std::string m_version;

        DeserializableValueContainer<TFormatType> m_valueContainer { };
    };

    // ================================================================
    // ================================================================
    // ================================================================
}

// including implementations
#include "Implementations/JSON/CommonImpl.h"

#endif //SUNGEARENGINE_SERDE_H
