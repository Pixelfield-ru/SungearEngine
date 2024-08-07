//
// Created by ilya on 24.06.24.
//

#ifndef SUNGEARENGINE_SERIALIZER_H
#define SUNGEARENGINE_SERIALIZER_H

#include <SGCore/pch.h>

#include "SGCore/Utils/TypeTraits.h"

#include "SGCore/Memory/Assets/IAsset.h"
#include "SGCore/Annotations/AnnotationsProcessor.h"

#include "SGCore/Utils/Event.h"

// HERE IS SPECIALIZATIONS OF SerializerSpec FOR BUILT-IN TYPES OF C++ AND LIBRARIES TYPES

namespace SGCore
{
    template<typename... Cls>
    struct BaseClasses
    {
        static constexpr std::uint64_t base_classes_count = sizeof...(Cls);

        template<std::int64_t Idx>
        using get_base_type = extract<Idx, Cls...>::type;
    };

    template<typename... Cls>
    struct DerivedClasses
    {
        static constexpr std::uint64_t derived_classes_count = sizeof...(Cls);

        template<std::int64_t Idx>
        using get_derived_type = extract<Idx, Cls...>::type;
    };

    /**
     * You must implement rapidjson_type and type_name
     * @tparam T
     */
    template<typename T>
    struct SerializerSpec : BaseClasses<>, DerivedClasses<>
    {
        static_assert(always_false<T>::value, "This type is not serializable. Please, implement specialization of SerializerSpec for this type.");

        static inline const rapidjson::Type rapidjson_type = rapidjson::kObjectType;
        static inline const std::string type_name = "unknown";

        /**
         * Call this if you want to serialize your value directly in value section\n
         * Serializes only data of value type without checking for derived types (no dynamic casts)
         * @param toDocument
         * @param parent
         * @param value
         */
        static void serialize(rapidjson::Document& toDocument,
                              rapidjson::Value& parentKey,
                              rapidjson::Value& parent,
                              rapidjson::Value& valueTypeName, // can be modified
                              const T& value) noexcept
        {
        }

        /**
         * Call this if you want to deserialize your value directly from value section\n
         * Deserializes only data of value type without checking for derived types (no dynamic casts)
         * @param toDocument
         * @param parent
         * @param value
         */
        static void deserialize(const rapidjson::Value& parent, const rapidjson::Value& value,
                                const std::string& typeName,
                                std::string& outputLog,
                                T& outputValue) noexcept
        {
        }
    };

    struct Serializer
    {
        template<typename T>
        struct Common
        {
            static Event<void(rapidjson::Document& toDocument,
                              rapidjson::Value& parent,
                              const T& value,
                              bool& isDynamicCastSuccessful)> doDynamicCasts;

            static Event<void(rapidjson::Value& parent,
                              rapidjson::Value& value,
                              const std::string& typeName,
                              bool& isTypeNamesChecksSuccessful,
                              T*& outputCastedValue)> doTypeNameChecks;
        };

        /**
         * Use if you want to serialize your data with 3rd party dynamic casts or type-checking before
         * @param toDocument
         * @param parent
         * @param varName
         * @param value
         */
        static void serializeUsing3rdPartyDynamicCasts(rapidjson::Document& toDocument,
                                                       rapidjson::Value& parentKey,
                                                       rapidjson::Value& parent,
                                                       rapidjson::Value& valueTypeName, // can be modified
                                                       const auto& value)
        {
            using type = std::remove_const_t<std::remove_reference_t<decltype(value)>>;
            using clear_type = clear_type_t<type>;

            if constexpr(std::is_pointer_v<type> || is_shared_ptr_v<type> || is_unique_ptr_v<type>)
            {
                if constexpr (std::is_class_v<clear_type>)
                {
                    bool dynamicCastsSuccessful = false;
                    // Common<clear_type>::doDynamicCasts(toDocument, parent, value, dynamicCastsSuccessful);

                    if (dynamicCastsSuccessful) return;
                }

                if constexpr(std::is_same_v<clear_type, const char*>)
                {
                    SerializerSpec<clear_type>::serialize(toDocument, parentKey, parent, valueTypeName, value);
                }
                else
                {
                    bool isAnyOfDerivedSerialized = false;
                    if(value)
                    {
                        // serializing all derived classes of <clear_type> class
                        serializeDerivedClasses<clear_type>(toDocument, parentKey, parent, valueTypeName, *value,
                                                            isAnyOfDerivedSerialized);

                        // serializing base classes
                        serializeBaseClasses<clear_type>(toDocument, parentKey, parent, valueTypeName, *value);\
                    }

                    // serializing only data of <type> class
                    SerializerSpec<type>::serialize(toDocument, parentKey, parent, valueTypeName, value);
                }
            }
            else
            {
                serializeBaseClasses<clear_type>(toDocument, parentKey, parent, valueTypeName, value);
                SerializerSpec<clear_type>::serialize(toDocument, parentKey, parent, valueTypeName, value);
            }
        }

        template<typename T>
        static void deserializeUsing3rdPartyDynamicCasts(const rapidjson::Value& parent,
                                                         const rapidjson::Value& value,
                                                         const std::string& typeName,
                                                         std::string& outputLog,
                                                         T& outputValue)
        {
            using clear_type = std::remove_const_t<std::remove_pointer_t<std::remove_reference_t<T>>>;

            if constexpr(std::is_pointer_v<T> || is_shared_ptr_v<T> || is_unique_ptr_v<T>)
            {
                if constexpr(std::is_class_v<clear_type>)
                {
                    bool typeNameChecksSuccessful = false;
                    T outputDynamicCastedPtr = nullptr;
                    // Common<clear_type>::doTypeNameChecks(parent, value, typeName, typeNameChecksSuccessful, outputDynamicCastedPtr);

                    if (typeNameChecksSuccessful && outputDynamicCastedPtr)
                    {
                        outputValue = std::move(outputDynamicCastedPtr);
                        return;
                    }
                }

                if constexpr(std::is_same_v<T, const char*>)
                {
                    // deserializing only this class`s type (T) types and its base types
                    SerializerSpec<clear_type>::deserializeStatic(parent, value, typeName, outputLog, outputValue);
                }
                else
                {
                    clear_type* out = nullptr;
                    tryDeserializeAsOneOfDerived<clear_type>(parent, value, typeName, outputLog, out);
                    // trying to deserialize only this type (T) value and its base classes values
                    if(!out)
                    {
                        deserializeBaseClasses<clear_type>(parent, value, typeName, outputLog, outputValue);

                        SerializerSpec<T>::deserialize(parent, value, typeName, outputLog, outputValue);
                    }
                }
            }
            else
            {
                // deserializing all base parts of type T
                deserializeBaseClasses<clear_type>(parent, value, typeName, outputLog, outputValue);

                SerializerSpec<clear_type>::deserialize(parent, value, typeName, outputLog, outputValue);
            }
        }

        template<typename T>
        static T deserializeUsing3rdPartyDynamicCasts(const rapidjson::Value& parent,
                                                      const rapidjson::Value& value,
                                                      const std::string& typeName,
                                                      std::string& outputLog)
        {
            T val { };
            deserializeUsing3rdPartyDynamicCasts(parent, value, typeName, outputLog, val);

            return val;
        }

        /**
         * Use if you want to serialize data with generation of sections "typename" and "value"
         * @param toDocument
         * @param parent
         * @param varName
         * @param value
         */
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, const auto& value) noexcept
        {
            using serializer_spec_t = SerializerSpec<std::remove_cvref_t<decltype(value)>>;

            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kObjectType);

            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

            addSections(toDocument, v, serializer_spec_t::type_name, value);

            switch (parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, v, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(v, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        /**
         * Use if you want to serialize data with generation of sections "typename" and "value"
         * @param toDocument
         * @param parent
         * @param varName
         * @param value
         */
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, auto& value) noexcept
        {
            using serializer_spec_t = SerializerSpec<std::remove_cvref_t<decltype(value)>>;

            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kObjectType);

            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

            addSections(toDocument, v, serializer_spec_t::type_name, value);

            switch (parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, v, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(v, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        /**
         * Use if you want to deserialize data with sections "typename" and "value"
         * @param toDocument
         * @param parent
         * @param varName
         * @param value
         */
        template<typename T>
        static void deserialize(const rapidjson::Value& parent, const std::string& varName, std::string& outputLog,
                                T& outputValue) noexcept
        {
            if (!parent.HasMember(varName.c_str()))
            {
                Serializer::formNotExistingMemberError(parent, varName, outputLog);

                return;
            }

            auto& self = parent[varName.c_str()];

            if(!self.HasMember("typename"))
            {
                Serializer::formNotExistingMemberError(parent, "typename", outputLog);

                return;
            }

            if(!self.HasMember("value"))
            {
                Serializer::formNotExistingMemberError(parent, "value", outputLog);

                return;
            }

            auto& typenameSection = self["typename"];
            auto& valueSection = self["value"];

            deserializeUsing3rdPartyDynamicCasts<T>(parent,
                                                    valueSection,
                                                    typenameSection.GetString(),
                                                    outputLog,
                                                    outputValue);
        }

        /**
         * Use if you want to deserialize data with sections "typename" and "value"
         * @param toDocument
         * @param parent
         * @param varName
         * @param value
         */
        template<typename T>
        static T deserialize(const rapidjson::Value& parent, const std::string& varName, std::string& outputLog) noexcept
        {
            T val { };
            deserialize<T>(parent, varName, outputLog, val);

            return val;
        }

        /**
         * Use if you want to deserialize data with sections "typename" and "value"
         * @param toDocument
         * @param parent
         * @param varName
         * @param value
         */
        template<typename T>
        static void deserialize(const rapidjson::Value& parent, const std::uint64_t& index, std::string& outputLog,
                                T& outputValue) noexcept
        {
            if (parent.Size() <= index)
            {
                Serializer::formNotExistingMemberError(parent, index, outputLog);

                return;
            }

            auto& self = parent[index];

            if(!self.HasMember("typename"))
            {
                Serializer::formNotExistingMemberError(parent, "typename", outputLog);

                return;
            }

            if(!self.HasMember("value"))
            {
                Serializer::formNotExistingMemberError(parent, "value", outputLog);

                return;
            }

            auto& typenameSection = self["typename"];
            auto& valueSection = self["value"];

            deserializeUsing3rdPartyDynamicCasts<T>(parent,
                                                    valueSection,
                                                    typenameSection.GetString(),
                                                    outputLog,
                                                    outputValue);
        }

        /**
         * Use if you want to deserialize data with sections "typename" and "value"
         * @param toDocument
         * @param parent
         * @param varName
         * @param value
         */
        template<typename T>
        static T deserialize(const rapidjson::Value& parent, const std::uint64_t& index, std::string& outputLog) noexcept
        {
            T val { };
            deserialize<T>(parent, index, outputLog, val);

            return val;
        }

        static void formNotExistingMemberError(const rapidjson::Value& parent, const std::string& varName, std::string& outputLog) noexcept
        {
            // TODO: MAYBE PARENT NAME
            outputLog += "Error: member '" + varName + "' does not exist";
        }

        static void formNotExistingMemberError(const rapidjson::Value& parent, const std::uint64_t& index, std::string& outputLog) noexcept
        {
            // TODO: MAYBE PARENT NAME
            outputLog += "Error: member with index '" + std::to_string(index) + "' does not exist";
        }

        static void addSections(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& typeName, const auto& value) noexcept
        {
            rapidjson::Value valueTypeK(rapidjson::kStringType);
            rapidjson::Value valueTypeV(rapidjson::kObjectType);

            valueTypeK.SetString("typename", 8, toDocument.GetAllocator());
            valueTypeV.SetString(typeName.c_str(), typeName.length(), toDocument.GetAllocator());
            // ======================================

            // adding value section. value section can modify content of typename section
            using value_t = std::remove_cvref_t<decltype(value)>;

            rapidjson::Value valueK(rapidjson::kStringType);
            rapidjson::Value valueV(rapidjson::kObjectType);

            valueK.SetString("value", 5, toDocument.GetAllocator());

            serializeUsing3rdPartyDynamicCasts(toDocument,
                                               valueK,
                                               valueV,
                                               valueTypeV,
                                               value);

            parent.AddMember(valueK, valueV, toDocument.GetAllocator());

            // ======================================
            parent.AddMember(valueTypeK, valueTypeV, toDocument.GetAllocator());
        }

    // private:
        // type and has element_type using
        template<typename T, bool HasElementType>
        struct clear_type_impl;

        template<typename T>
        struct clear_type_impl<T, true>
        {
            using type = typename T::element_type;
        };

        template<typename T>
        struct clear_type_impl<T, false>
        {
            using type = std::remove_const_t<std::remove_pointer_t<std::remove_reference_t<T>>>;
        };

        template<typename T>
        using clear_type_t = clear_type_impl<T, requires { typename T::element_type; }>::type;

        template<typename DerivedT, typename BaseT>
        static void deserializeBaseClass(const rapidjson::Value& parent,
                                         const rapidjson::Value& value,
                                         const std::string& typeName,
                                         std::string& outputLog,
                                         DerivedT*& outputValue)
        {
            SerializerSpec<BaseT>::deserialize(parent, value, typeName, outputLog, *outputValue);
        }

        template<typename DerivedT, std::int64_t... Indices>
        static void deserializeBaseClassesImpl(const rapidjson::Value& parent,
                                               const rapidjson::Value& value,
                                               const std::string& typeName,
                                               std::string& outputLog,
                                               DerivedT*& outputValue,
                                               std::index_sequence<Indices...>)
        {
            (deserializeBaseClass<DerivedT, SerializerSpec<DerivedT>::template get_base_type<Indices>>(parent, value,
                                                                                                       typeName,
                                                                                                       outputLog,
                                                                                                       outputValue), ...);
        }

        template<typename DerivedT>
        static void deserializeBaseClasses(const rapidjson::Value& parent,
                                           const rapidjson::Value& value,
                                           const std::string& typeName,
                                           std::string& outputLog,
                                           DerivedT*& outputValue)
        {
            if constexpr(requires { SerializerSpec<DerivedT>::base_classes_count && SerializerSpec<DerivedT>::get_base_type; })
            {
                deserializeBaseClassesImpl<DerivedT>(parent, value, typeName, outputLog, outputValue,
                                                     std::make_index_sequence<SerializerSpec<DerivedT>::base_classes_count> {});
            }
        }

        template<typename BaseT, typename DerivedT>
        static void tryDeserializeAsDerived(const rapidjson::Value& parent,
                                            const rapidjson::Value& value,
                                            const std::string& typeName,
                                            std::string& outputLog,
                                            BaseT*& outputValue)
        {
            if(typeName == SerializerSpec<DerivedT>::type_name)
            {
                assert(!outputValue && fmt::format(
                        "Some two or more serializer specializations contain the same value '{0}' for the type_name field."
                        " The class '{1}' cannot be deserialized correctly.", SerializerSpec<DerivedT>::type_name,
                        SerializerSpec<BaseT>::type_name));

                // creating instance of derived
                outputValue = new DerivedT();

                // deserializing all base parts of derived type (DerivedT)
                deserializeBaseClasses<DerivedT>(parent, value, typeName, outputLog, outputValue);

                // deserializing only values of derived type (DerivedT)
                SerializerSpec<DerivedT>::deserialize(parent, value, typeName, outputLog, outputValue);
            }
            else
            {
                // go ahead and try to deserialize as one of DerivedT derived classes
                tryDeserializeAsOneOfDerived<DerivedT>(parent, value, typeName, outputLog, outputValue);
            }
        }

        template<typename BaseT, std::int64_t... Indices>
        static void tryDeserializeAsOneOfDerivedImpl(const rapidjson::Value& parent,
                                                     const rapidjson::Value& value,
                                                     const std::string& typeName,
                                                     std::string& outputLog,
                                                     BaseT*& outputValue,
                                                     std::index_sequence<Indices...>)
        {
            (tryDeserializeAsDerived<BaseT, SerializerSpec<BaseT>::template get_derived_type<Indices>>(parent, value,
                                                                                                       typeName,
                                                                                                       outputLog,
                                                                                                       outputValue), ...);
        }

        template<typename BaseT>
        static void tryDeserializeAsOneOfDerived(const rapidjson::Value& parent,
                                                 const rapidjson::Value& value,
                                                 const std::string& typeName,
                                                 std::string& outputLog,
                                                 BaseT*& outputValue)
        {
            if constexpr(requires { SerializerSpec<BaseT>::derived_classes_count && SerializerSpec<BaseT>::get_derived_type; })
            {
                tryDeserializeAsOneOfDerivedImpl<BaseT>(parent, value, typeName, outputLog, outputValue,
                                                        std::make_index_sequence<SerializerSpec<BaseT>::derived_classes_count> {});
            }
        }

        template<typename BaseT, typename DerivedT>
        static void trySerializeDerivedClass(rapidjson::Document& toDocument,
                                             rapidjson::Value& parentKey,
                                             rapidjson::Value& parent,
                                             rapidjson::Value& valueTypeName, // can be modified
                                             const BaseT& value,
                                             bool& isAnyOfDerivedSerialized)
        {
            const auto* derived = dynamic_cast<const DerivedT*>(&value);
            if(derived)
            {
                isAnyOfDerivedSerialized = true;
                // setting new typename
                valueTypeName.SetString(SerializerSpec<DerivedT>::type_name.c_str(),
                                        SerializerSpec<DerivedT>::type_name.length());

                // serializing all derived classes of DerivedT class
                serializeDerivedClasses<DerivedT>(toDocument, parentKey, parent, valueTypeName, *derived,
                                                    isAnyOfDerivedSerialized);

                // serializing only data of DerivedT class
                SerializerSpec<DerivedT>::serialize(toDocument, parentKey, parent, valueTypeName, *derived);
            }
        }

        template<typename T, std::int64_t... Indices>
        static void serializeDerivedClassesImpl(rapidjson::Document& toDocument,
                                                rapidjson::Value& parentKey,
                                                rapidjson::Value& parent,
                                                rapidjson::Value& valueTypeName, // can be modified
                                                const T& value,
                                                bool& isAnyOfDerivedSerialized,
                                                std::index_sequence<Indices...>)
        {
            (trySerializeDerivedClass<T, typename SerializerSpec<T>::template get_derived_type<Indices>>(toDocument,
                                                                                                         parentKey,
                                                                                                         parent,
                                                                                                         valueTypeName,
                                                                                                         value,
                                                                                                         isAnyOfDerivedSerialized), ...);
        }

        template<typename T>
        static void serializeDerivedClasses(rapidjson::Document& toDocument,
                                            rapidjson::Value& parentKey,
                                            rapidjson::Value& parent,
                                            rapidjson::Value& valueTypeName, // can be modified
                                            const T& value,
                                            bool& isAnyOfDerivedSerialized)
        {
            if constexpr(requires { SerializerSpec<T>::derived_classes_count && SerializerSpec<T>::get_derived_type; })
            {
                serializeDerivedClassesImpl<T>(toDocument, parentKey, parent, valueTypeName, value,
                                               isAnyOfDerivedSerialized,
                                               std::make_index_sequence<SerializerSpec<T>::derived_classes_count> {});
            }
        }

        template<typename T, std::int64_t... Indices>
        static void serializeBaseClassesImpl(rapidjson::Document& toDocument,
                                             rapidjson::Value& parentKey,
                                             rapidjson::Value& parent,
                                             rapidjson::Value& valueTypeName, // can be modified
                                             const T& value,
                                             std::index_sequence<Indices...>)
        {
            (serializeUsing3rdPartyDynamicCasts(toDocument, parentKey, parent, valueTypeName,
                                               static_cast<const SerializerSpec<T>::template get_base_type<Indices>&>(value)), ...);
        }

        template<typename T>
        static void serializeBaseClasses(rapidjson::Document& toDocument,
                                         rapidjson::Value& parentKey,
                                         rapidjson::Value& parent,
                                         rapidjson::Value& valueTypeName, // can be modified
                                         const T& value)
        {
            if constexpr(requires { SerializerSpec<T>::base_classes_count && SerializerSpec<T>::get_base_type; })
            {
                serializeBaseClassesImpl<T>(toDocument, parentKey, parent, valueTypeName, value,
                                            std::make_index_sequence<SerializerSpec<T>::base_classes_count> {});
            }
        }
    };

    // ===============================================================================================================================
    // ===============================================================================================================================
    // ===============================================================================================================================
    // STANDARD SERIALIZABLES FOR TYPES

    template<typename T>
    requires(std::is_enum_v<T>)
    struct SerializerSpec<T>
    {
        static inline const rapidjson::Type rapidjson_type = rapidjson::kNumberType;
        static inline const std::string type_name = "enum";

        static void serialize(rapidjson::Document& toDocument,
                              rapidjson::Value& parentKey,
                              rapidjson::Value& parent,
                              rapidjson::Value& valueTypeName,
                              const T& value) noexcept
        {
            parent.SetInt(static_cast<std::int64_t>(value));
        }

        static void deserializeDynamic(const rapidjson::Value& parent,
                                       const rapidjson::Value& value,
                                       const std::string& typeName,
                                       std::string& outputLog,
                                       T*& outputValue) noexcept
        {
            outputValue = new T(static_cast<T>(value.GetInt64()));
        }

        static void deserializeStatic(const rapidjson::Value& parent,
                                      const rapidjson::Value& value,
                                      const std::string& typeName,
                                      std::string& outputLog,
                                      T& outputValue) noexcept
        {
            outputValue = static_cast<T>(value.GetInt64());
        }
    };

    template<typename T>
    struct SerializerSpec<T*>
    {
        using element_type = std::remove_const_t<std::remove_pointer_t<std::remove_reference_t<T>>>;

        static inline const rapidjson::Type rapidjson_type = SerializerSpec<element_type>::rapidjson_type;
        static inline const std::string type_name = SerializerSpec<element_type>::type_name;


        static void serialize(rapidjson::Document& toDocument,
                              rapidjson::Value& parentKey,
                              rapidjson::Value& parent,
                              rapidjson::Value& valueTypeName,
                              const T* value) noexcept
        {
            if(value)
            {
                SerializerSpec<element_type>::serialize(toDocument,
                                                        parentKey,
                                                        parent,
                                                        valueTypeName,
                                                        *value);
            }
            else
            {
                parent = rapidjson::Value(rapidjson::kNullType);
            }
        }

        static void deserializeDynamic(const rapidjson::Value& parent,
                                       const rapidjson::Value& value,
                                       const std::string& typeName,
                                       std::string& outputLog,
                                       T*& outputValue) noexcept
        {
            if (value.IsNull())
            {
                outputValue = nullptr;
                return;
            }

            SerializerSpec<element_type>::deserializeDynamic(parent, value, typeName, outputLog, outputValue);
        }

        // IMPOSSIBLE OPTION
//        static T deserializeStatic(const rapidjson::Value& parent, const rapidjson::Value& value,
//                                   const std::string& typeName,
//                                   std::string& outputLog) noexcept
//        {
//        }
    };

    template<typename T>
    struct SerializerSpec<std::shared_ptr<T>>
    {
        using element_type = T;

        static inline const rapidjson::Type rapidjson_type = SerializerSpec<element_type>::rapidjson_type;
        static inline const std::string type_name = SerializerSpec<element_type>::type_name;

        static void serialize(rapidjson::Document& toDocument,
                              rapidjson::Value& parentKey,
                              rapidjson::Value& parent,
                              rapidjson::Value& valueTypeName,
                              const std::shared_ptr<T>& value) noexcept
        {
            if (value)
            {
                SerializerSpec<element_type>::serialize(toDocument,
                                                        parentKey,
                                                        parent,
                                                        valueTypeName,
                                                        *value);
            }
            else
            {
                parent = rapidjson::Value(rapidjson::kNullType);
            }
        }

        static void deserializeDynamic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                       const std::string& typeName,
                                       std::string& outputLog,
                                       std::shared_ptr<T>& outputValue) noexcept
        {
            if (value.IsNull())
            {
                outputValue = nullptr;
                return;
            }

            element_type* tmp = nullptr;
            SerializerSpec<element_type>::deserializeDynamic(parent, value, typeName, outputLog, tmp);
            outputValue = std::shared_ptr<element_type>(tmp);
        }

        // IMPOSSIBLE OPTION
//        static T deserializeStatic(const rapidjson::Value& parent, const rapidjson::Value& value,
//                                   const std::string& typeName,
//                                   std::string& outputLog) noexcept
//        {
//        }
    };

    template<typename T>
    struct SerializerSpec<std::unique_ptr<T>>
    {
        using element_type = T;

        static inline const rapidjson::Type rapidjson_type = SerializerSpec<element_type>::rapidjson_type;
        static inline const std::string type_name = SerializerSpec<element_type>::type_name;

        static void serialize(rapidjson::Document& toDocument,
                              rapidjson::Value& parentKey,
                              rapidjson::Value& parent,
                              rapidjson::Value& valueTypeName,
                              const std::unique_ptr<T>& value) noexcept
        {
            if (value)
            {
                SerializerSpec<element_type>::serialize(toDocument,
                                                        parentKey,
                                                        parent,
                                                        valueTypeName,
                                                        *value);
            }
            else
            {
                parent = rapidjson::Value(rapidjson::kNullType);
            }
        }

        static void deserializeDynamic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                       const std::string& typeName,
                                       std::string& outputLog,
                                       std::unique_ptr<T>& outputValue) noexcept
        {
            if(value.IsNull())
            {
                outputValue = nullptr;
                return;
            }

            element_type* tmp = nullptr;
            SerializerSpec<element_type>::deserializeDynamic(parent, value, typeName, outputLog, tmp);
            outputValue = std::unique_ptr<element_type>(tmp);
        }

        // IMPOSSIBLE OPTION
//        static T deserializeStatic(const rapidjson::Value& parent, const rapidjson::Value& value,
//                                   const std::string& typeName,
//                                   std::string& outputLog) noexcept
//        {
//        }
    };

    template<>
    struct SerializerSpec<char>
    {
        static inline const rapidjson::Type rapidjson_type = rapidjson::kStringType;
        static inline const std::string type_name = "char";


        static void serialize(rapidjson::Document& toDocument,
                              rapidjson::Value& parentKey,
                              rapidjson::Value& parent,
                              rapidjson::Value& valueTypeName,
                              const char& value) noexcept
        {
            parent.AddMember(parentKey, value, toDocument.GetAllocator());
        }

        static void deserializeDynamic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                       const std::string& typeName,
                                       std::string& outputLog,
                                       char*& outputValue) noexcept
        {
            outputValue = new char(value.GetStringLength() > 0 ? value.GetString()[0] : ' ');
        }

        static void deserializeStatic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                      const std::string& typeName,
                                      std::string& outputLog,
                                      char& outputValue) noexcept
        {
            outputValue = value.GetStringLength() > 0 ? value.GetString()[0] : ' ';
        }
    };

    template<>
    struct SerializerSpec<std::int8_t>
    {
        static inline const rapidjson::Type rapidjson_type = rapidjson::kNumberType;
        static inline const std::string type_name = "std::int8_t";

        static void serialize(rapidjson::Document& toDocument,
                              rapidjson::Value& parentKey,
                              rapidjson::Value& parent,
                              rapidjson::Value& valueTypeName,
                              const std::int8_t& value) noexcept
        {
            parent.SetInt(value);
        }

        static void deserializeDynamic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                       const std::string& typeName,
                                       std::string& outputLog,
                                       std::int8_t*& outputValue) noexcept
        {
            outputValue = new std::int8_t(value.GetInt());
        }

        static void deserializeStatic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                      const std::string& typeName,
                                      std::string& outputLog,
                                      std::int8_t& outputValue) noexcept
        {
            outputValue = value.GetInt();
        }
    };

    template<>
    struct SerializerSpec<std::int16_t>
    {
        static inline const rapidjson::Type rapidjson_type = rapidjson::kNumberType;
        static inline const std::string type_name = "std::int16_t";

        static void serialize(rapidjson::Document& toDocument,
                              rapidjson::Value& parentKey,
                              rapidjson::Value& parent,
                              rapidjson::Value& valueTypeName,
                              const std::int16_t& value) noexcept
        {
            parent.SetInt(value);
        }

        static void deserializeDynamic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                       const std::string& typeName,
                                       std::string& outputLog,
                                       std::int16_t*& outputValue) noexcept
        {
            outputValue = new std::int16_t(value.GetInt());
        }

        static void deserializeStatic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                      const std::string& typeName,
                                      std::string& outputLog,
                                      std::int16_t& outputValue) noexcept
        {
            outputValue = value.GetInt();
        }
    };

    template<>
    struct SerializerSpec<std::int32_t>
    {
        static inline const rapidjson::Type rapidjson_type = rapidjson::kNumberType;
        static inline const std::string type_name = "std::int32_t";

        static void serialize(rapidjson::Document& toDocument,
                              rapidjson::Value& parentKey,
                              rapidjson::Value& parent,
                              rapidjson::Value& valueTypeName,
                              const std::int32_t& value) noexcept
        {
            parent.SetInt(value);
        }

        static void serializeStatic(rapidjson::Document& toDocument,
                                    rapidjson::Value& parentKey,
                                    rapidjson::Value& parent,
                                    rapidjson::Value& valueTypeName,
                                    const std::int32_t& value) noexcept
        {
            parent.SetInt(value);
        }

        static void deserializeDynamic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                       const std::string& typeName,
                                       std::string& outputLog,
                                       std::int32_t*& outputValue) noexcept
        {
            outputValue = new std::int32_t(value.GetInt());
        }

        static void deserializeStatic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                      const std::string& typeName,
                                      std::string& outputLog,
                                      std::int32_t& outputValue) noexcept
        {
            outputValue = value.GetInt();
        }
    };

    template<>
    struct SerializerSpec<std::int64_t>
    {
        static inline const rapidjson::Type rapidjson_type = rapidjson::kNumberType;
        static inline const std::string type_name = "std::int64_t";

        static void serialize(rapidjson::Document& toDocument,
                              rapidjson::Value& parentKey,
                              rapidjson::Value& parent,
                              rapidjson::Value& valueTypeName,
                              const std::int64_t& value) noexcept
        {
            parent.SetInt64(value);
        }

        static void serializeStatic(rapidjson::Document& toDocument,
                                    rapidjson::Value& parentKey,
                                    rapidjson::Value& parent,
                                    rapidjson::Value& valueTypeName,
                                    const std::int64_t& value) noexcept
        {
            parent.SetInt64(value);
        }

        static void deserializeDynamic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                       const std::string& typeName,
                                       std::string& outputLog,
                                       std::int64_t*& outputValue) noexcept
        {
            outputValue = new std::int64_t(value.GetInt64());
        }

        static void deserializeStatic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                      const std::string& typeName,
                                      std::string& outputLog,
                                      std::int64_t& outputValue) noexcept
        {
            outputValue = value.GetInt64();
        }
    };

    template<>
    struct SerializerSpec<float>
    {
        static inline const rapidjson::Type rapidjson_type = rapidjson::kNumberType;
        static inline const std::string type_name = "float";

        static void serialize(rapidjson::Document& toDocument,
                              rapidjson::Value& parentKey,
                              rapidjson::Value& parent,
                              rapidjson::Value& valueTypeName,
                              const float& value) noexcept
        {
            parent.SetFloat(value);
        }

        static void serializeStatic(rapidjson::Document& toDocument,
                                    rapidjson::Value& parentKey,
                                    rapidjson::Value& parent,
                                    rapidjson::Value& valueTypeName,
                                    const float& value) noexcept
        {
            parent.SetFloat(value);
        }

        static void deserializeDynamic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                       const std::string& typeName,
                                       std::string& outputLog,
                                       float*& outputValue) noexcept
        {
            outputValue = new float(value.GetFloat());
        }

        static void deserializeStatic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                      const std::string& typeName,
                                      std::string& outputLog,
                                      float& outputValue) noexcept
        {
            outputValue = value.GetFloat();
        }
    };

    /*
    
    template<>
    struct SerializerSpec<std::uint8_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::uint8_t& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static std::uint8_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetUint();
        }
    };
    
    template<>
    struct SerializerSpec<std::uint16_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, const std::int16_t& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static std::uint16_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetUint();
        }
    };
    
    template<>
    struct SerializerSpec<std::uint32_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::uint32_t& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static std::uint32_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetUint();
        }
    };
    
    template<>
    struct SerializerSpec<std::uint64_t>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::uint64_t& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static std::uint64_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetUint64();
        }
    };
    
    template<>
    struct SerializerSpec<double>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const double& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static double deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetDouble();
        }
    };
    
    template<>
    struct SerializerSpec<float>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const float& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static float deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetFloat();
        }
    };

    template<>
    struct SerializerSpec<bool>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const bool& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, value, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(value, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static bool deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetBool();
        }
    };
    
    template<std::int32_t ScalarsCnt, typename ScalarT, glm::qualifier Qualifier>
    struct SerializerSpec<glm::vec<ScalarsCnt, ScalarT, Qualifier>>
    {
        using vec_t = glm::vec<ScalarsCnt, ScalarT, Qualifier>;

        static_assert(ScalarsCnt >= 1 && ScalarsCnt <= 4 && "Scalars count in glm::vec must be in range of 1-4.");

        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const vec_t& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kArrayType);
            
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            v.PushBack(value.x, toDocument.GetAllocator());
            if constexpr(ScalarsCnt >= 2)
            {
                v.PushBack(value.y, toDocument.GetAllocator());
            }
            if constexpr(ScalarsCnt >= 3)
            {
                v.PushBack(value.z, toDocument.GetAllocator());
            }
            if constexpr(ScalarsCnt >= 4)
            {
                v.PushBack(value.w, toDocument.GetAllocator());
            }
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, v, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(v, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static vec_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            vec_t outputValue;

            outputValue.x = value["x"];
            if constexpr(ScalarsCnt >= 2)
            {
                outputValue.y = value["y"];
            }
            if constexpr(ScalarsCnt >= 3)
            {
                outputValue.z = value["z"];
            }
            if constexpr(ScalarsCnt >= 4)
            {
                outputValue.w = value["w"];
            }

            return outputValue;
        }
    };
    
    template<std::int32_t ColumnsCnt, std::int32_t RowsCnt, typename ScalarT, glm::qualifier Qualifier>
    struct SerializerSpec<glm::mat<ColumnsCnt, RowsCnt, ScalarT, Qualifier>>
    {
        using mat_t = glm::mat<ColumnsCnt, RowsCnt, ScalarT, Qualifier>;

        static_assert(ColumnsCnt >= 1 && ColumnsCnt <= 4 && "Columns count in glm::mat must be in range of 1-4.");
        static_assert(RowsCnt >= 1 && RowsCnt <= 4 && "Rows count in glm::mat must be in range of 1-4.");
        
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const mat_t& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kArrayType);
            
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            for(std::int32_t c = 0; c < ColumnsCnt; ++c)
            {
                for(std::int32_t r = 0; r < RowsCnt; ++r)
                {
                    v.PushBack(value[c][r], toDocument.GetAllocator());
                }
            }
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, v, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(v, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static mat_t deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            mat_t outputValue;

            for(std::int32_t c = 0; c < ColumnsCnt; ++c)
            {
                for(std::int32_t r = 0; r < RowsCnt; ++r)
                {
                    outputValue[c][r] = value[c * ColumnsCnt + r];
                }
            }

            return outputValue;
        }
    };
    
    template<typename T>
    struct SerializerSpec<std::vector<T>>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::vector<T>& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kArrayType);
            
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            for(const auto& n : value)
            {
                Serializer::serialize(toDocument, v, "", n);
            }

            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, v, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(v, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static std::vector<T> deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            std::vector<T> outputValue;

            for(std::uint64_t i = 0; i < value.Size(); ++i)
            {
                outputValue.push_back(SerializerSpec<T>::deserialize(value, value[i], outputLog));
            }

            return outputValue;
        }
    };

    /// KeyT REQUIRES AN IMPLICIT CONVERSION OPERATOR TO std::string OR OTHER TYPES FROM
    /// WHICH std::string CAN BE CONSTRUCTED OR WHICH CAN BE IMPLICITLY CONVERTED TO std::string
    template<typename KeyT, typename ValueT>
    struct SerializerSpec<std::unordered_map<KeyT, ValueT>>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::unordered_map<KeyT, ValueT>& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kObjectType);

            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

            for(const auto& n : value)
            {
                Serializer::serialize(toDocument, v, n.first, n.second);
            }

            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, v, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(v, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            };
        }

        static std::unordered_map<KeyT, ValueT> deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            std::unordered_map<KeyT, ValueT> outputValue;

            for (rapidjson::Value::ConstMemberIterator iter = value.MemberBegin(); iter != value.MemberEnd(); ++iter)
            {
                outputValue[iter->name.GetString()] = SerializerSpec<ValueT>::deserialize(value, iter->value, outputLog);
            }

            return outputValue;
        }
    };
    
    template<>
    struct SerializerSpec<std::string>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::string& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kStringType);

            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            v.SetString(value.c_str(), value.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, v, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(v, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static std::string deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetString();
        }
    };

    template<>
    struct SerializerSpec<std::basic_string<char16_t>>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::basic_string<char16_t>& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kStringType);

            auto asUTF8 = Utils::toUTF8<char16_t>(value);

            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            v.SetString(asUTF8.c_str(), asUTF8.length(), toDocument.GetAllocator());

            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, v, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(v, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static std::basic_string<char16_t> deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return Utils::fromUTF8<char16_t>(value.GetString());
        }
    };

    template<>
    struct SerializerSpec<std::filesystem::path>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const std::filesystem::path& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kStringType);

            auto asUTF8 = Utils::toUTF8<char16_t>(value.u16string());

            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            v.SetString(asUTF8.c_str(), asUTF8.length(), toDocument.GetAllocator());

            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, v, toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(v, toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static std::filesystem::path deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetString();
        }
    };
    
    template<>
    struct SerializerSpec<const char*>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const char* value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());
            
            switch(parent.GetType())
            {
                case rapidjson::kNullType:
                    break;
                case rapidjson::kFalseType:
                    break;
                case rapidjson::kTrueType:
                    break;
                case rapidjson::kObjectType:
                    parent.AddMember(k, rapidjson::StringRef(value), toDocument.GetAllocator());
                    break;
                case rapidjson::kArrayType:
                    parent.PushBack(rapidjson::StringRef(value), toDocument.GetAllocator());
                    break;
                case rapidjson::kStringType:
                    break;
                case rapidjson::kNumberType:
                    break;
            }
        }

        static const char* deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            return value.GetString();
        }
    };

    template<typename T>
    concept CustomSerializationStruct =
    requires(T obj,
            rapidjson::Document& document,
            rapidjson::Value& parent,
            const std::string& varName,
            const rapidjson::Value& jsonValue,
            std::string& outputLog) {
        obj.serializeData(document, parent, varName);
        obj.serializeMeta(document, parent, varName);
        obj.deserializeData(parent, jsonValue, outputLog);
        obj.deserializeMeta(parent, jsonValue, outputLog);
        obj.m_serializationType;
    };

    template<CustomSerializationStruct T>
    struct SerializerSpec<T>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, T& value) noexcept
        {
            switch(value.m_serializationType)
            {
                case SerializationType::SERIALIZE_META:
                    value.serializeMeta(toDocument, parent, varName);
                    break;
                case SerializationType::SERIALIZE_DATA:
                    value.serializeData(toDocument, parent, varName);
                    break;
            }
        }

        static T deserialize(const rapidjson::Value& parent, const rapidjson::Value& value, std::string& outputLog) noexcept
        {
            T outputValue;

            switch(outputValue.m_serializationType)
            {
                case SerializationType::SERIALIZE_META:
                    outputValue.deserializeMeta(parent, value, outputLog);
                    break;
                case SerializationType::SERIALIZE_DATA:
                    outputValue.deserializeData(parent, value, outputLog);
                    break;
            }

            return outputValue;
        }
    };

    template<>
    struct SerializerSpec<AnnotationsProcessor::AnnotationArg>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName,
                              const AnnotationsProcessor::AnnotationArg& value) noexcept;

        static AnnotationsProcessor::AnnotationArg deserialize(const rapidjson::Value& parent,
                                                               const rapidjson::Value& value,
                                                               std::string& outputLog) noexcept;
    };

    template<>
    struct SerializerSpec<AnnotationsProcessor::Annotation>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName,
                              const AnnotationsProcessor::Annotation& value) noexcept;

        static AnnotationsProcessor::Annotation deserialize(const rapidjson::Value& parent,
                                                            const rapidjson::Value& value,
                                                            std::string& outputLog) noexcept;
    };

    template<>
    struct SerializerSpec<AnnotationsProcessor::Member>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName,
                              const AnnotationsProcessor::Member& value) noexcept;

        static AnnotationsProcessor::Member deserialize(const rapidjson::Value& parent,
                                                        const rapidjson::Value& value,
                                                        std::string& outputLog) noexcept;
    };

    template<>
    struct SerializerSpec<AnnotationsProcessor>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName,
                              const AnnotationsProcessor& value) noexcept;

        static AnnotationsProcessor deserialize(const rapidjson::Value& parent,
                                                const rapidjson::Value& value,
                                                std::string& outputLog) noexcept;
    };*/
}

#endif //SUNGEARENGINE_SERIALIZER_H
