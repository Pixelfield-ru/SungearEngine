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
    namespace Serde
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

        // DO NOT STORE ANYWHERE
        // THIS STRUCT DOES NOT OWN ANYTHING
        template<typename T>
        struct ValueDataView
        {
            rapidjson::Document*    /*const*/ m_document {};
            rapidjson::Value*       /*const*/ m_parentKey {};
            rapidjson::Value*       /*const*/ m_parentValue {};
            rapidjson::Value*       /*const*/ m_valueTypeName {};
            rapidjson::Value*       /*const*/ m_value {};
            std::string* m_log { };
            T* m_outputValue {};

            ValueDataView() = default;
            ValueDataView(const ValueDataView&) = default;
            ValueDataView(ValueDataView&&) = default;

            template<typename T0>
            ValueDataView(const ValueDataView<T0>& rhs) noexcept
            {
                *this = rhs;
            }

            template<typename T0>
            ValueDataView<T>& operator=(ValueDataView<T0>& rhs) noexcept
            {
                m_document = rhs.m_document;
                m_parentKey = rhs.m_parentKey;
                m_parentValue = rhs.m_parentValue;
                m_valueTypeName = rhs.m_valueTypeName;
                m_value = rhs.m_value;
                m_log = rhs.m_log;
                m_outputValue = (T*) rhs.m_outputValue;

                return *this;
            }

            template<typename T0>
            ValueDataView<T0> copyMeta() noexcept
            {
                ValueDataView<T0> tmp { };
                tmp.m_document = m_document;
                tmp.m_parentKey = m_parentKey;
                tmp.m_parentValue = m_parentValue;
                tmp.m_valueTypeName = m_valueTypeName;
                tmp.m_value = m_value;
                tmp.m_log = m_log;

                return tmp;
            }
        };

        /**
         * You must implement rapidjson_type and type_name
         * @tparam T
         */
        template<typename T>
        struct SerializerSpec : BaseClasses<>, DerivedClasses<>
        {
            static_assert(always_false<T>::value,
                          "This type is not serializable. Please, implement specialization of SerializerSpec for this type.");

            static inline const rapidjson::Type rapidjson_type = rapidjson::kObjectType;
            static inline const std::string type_name = "unknown";

            /**
             * Call this if you want to serialize your value directly in value section\n
             * Serializes only data of value type
             */
            static void serialize(ValueDataView<T>& valueData) noexcept
            {
            }

            /**
             * Call this if you want to deserialize your value directly from value section\n
             * Deserializes only data of value type without checking for derived types (no dynamic casts)
             * @param toDocument
             * @param parent
             * @param value
             */
            static void deserialize(ValueDataView<T>& valueData) noexcept
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
             */
            template<typename T>
            static void serializeUsing3rdPartyDynamicCasts(ValueDataView<T>& valueData)
            {
                using clear_type = clear_type_t<T>;

                if constexpr (std::is_pointer_v<T> || is_shared_ptr_v<T> || is_unique_ptr_v<T>)
                {
                    if constexpr (std::is_class_v<clear_type>)
                    {
                        bool dynamicCastsSuccessful = false;
                        // Common<clear_type>::doDynamicCasts(toDocument, parent, value, dynamicCastsSuccessful);

                        if (dynamicCastsSuccessful) return;
                    }

                    if constexpr (std::is_same_v<clear_type, const char*>)
                    {
                        SerializerSpec<clear_type>::serialize(valueData);
                    }
                    else
                    {
                        bool isAnyOfDerivedSerialized = false;
                        if (valueData.m_outputValue)
                        {
                            ValueDataView<clear_type> tmpData = valueData.template copyMeta<clear_type>();
                            tmpData.m_outputValue = &*valueData.m_outputValue;

                            // serializing all derived classes of <clear_type> class
                            serializeDerivedClasses<clear_type>(tmpData,
                                                                isAnyOfDerivedSerialized);

                            // serializing base classes
                            serializeBaseClasses<clear_type>(tmpData);

                        }

                        // serializing only data of <type> class
                        SerializerSpec<T>::serialize(valueData);
                    }
                }
                else
                {
                    serializeBaseClasses<clear_type>(valueData);
                    SerializerSpec<clear_type>::serialize(valueData);
                }
            }

            template<typename T>
            static void deserializeUsing3rdPartyDynamicCasts(ValueDataView<T>& valueData)
            {
                using clear_type = clear_type_t<T>;
                // using clear_type = std::remove_const_t<std::remove_pointer_t<std::remove_reference_t<T>>>;

                if constexpr (std::is_pointer_v<T> || is_shared_ptr_v<T> || is_unique_ptr_v<T>)
                {
                    if constexpr (std::is_class_v<clear_type>)
                    {
                        bool typeNameChecksSuccessful = false;
                        T outputDynamicCastedPtr = nullptr;
                        // Common<clear_type>::doTypeNameChecks(parent, value, typeName, typeNameChecksSuccessful, outputDynamicCastedPtr);

                        if (typeNameChecksSuccessful && outputDynamicCastedPtr)
                        {
                            // valueData.m_outputValue = outputDynamicCastedPtr;
                            return;
                        }
                    }

                    if constexpr (std::is_same_v<T, const char*>)
                    {
                        // deserializing only this class`s type (T) types and its base types
                        SerializerSpec<T>::deserializeStatic(valueData);
                    }
                    else
                    {
                        ValueDataView<clear_type> tmpData = valueData.template copyMeta<clear_type>();
                        tmpData.m_outputValue = valueData.m_outputValue;

                        // first trying to deserialize as one of deriving types
                        tryDeserializeAsOneOfDerived<clear_type>(tmpData);
                        // trying to deserialize only this type (T) value and its base classes values
                        if (!*tmpData.m_outputValue)
                        {
                            // trying to deserialize only if type_name's equal
                            if (SerializerSpec<clear_type>::type_name == valueData.m_valueTypeName->GetString())
                            {
                                // allocating
                                // tmpData.m_outputValue = new clear_type();

                                SerializerSpec<clear_type>::deserialize(tmpData);

                                deserializeBaseClasses<clear_type>(tmpData);
                            }
                        }

                        if constexpr (std::is_pointer_v<T>)
                        {
                            valueData.m_outputValue = tmpData.m_outputValue;
                        }
                        else // if other pointer type
                        {
                            valueData.m_outputValue = std::move(T(tmpData.m_outputValue));
                        }
                    }
                }
                else
                {
                    // deserializing all base parts of type T
                    deserializeBaseClasses<clear_type>(valueData);

                    SerializerSpec<clear_type>::deserialize(valueData);
                }
            }

            /**
             * Use if you want to serialize data with generation of sections "typename" and "value"
             */
            template<typename T>
            static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName,
                                  const T& value) noexcept
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
            template<typename T>
            static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName,
                                  T& value) noexcept
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
            static void deserialize(rapidjson::Value& parent, const std::string& varName, std::string& outputLog,
                                    T& outputValue) noexcept
            {
                if (!parent.HasMember(varName.c_str()))
                {
                    Serializer::formNotExistingMemberError(parent, varName, outputLog);

                    return;
                }

                auto& self = parent[varName.c_str()];

                if (!self.HasMember("typename"))
                {
                    Serializer::formNotExistingMemberError(parent, "typename", outputLog);

                    return;
                }

                if (!self.HasMember("value"))
                {
                    Serializer::formNotExistingMemberError(parent, "value", outputLog);

                    return;
                }

                auto& typenameSection = self["typename"];
                auto& valueSection = self["value"];

                ValueDataView<T> outputValueData;
                outputValueData.m_parentValue = &parent;
                outputValueData.m_valueTypeName = &typenameSection;
                outputValueData.m_value = &valueSection;
                outputValueData.m_log = &outputLog;
                outputValueData.m_outputValue = &outputValue;

                deserializeUsing3rdPartyDynamicCasts<T>(outputValueData);
            }

            template<typename T>
            static T deserialize(rapidjson::Value& parent, const std::string& varName, std::string& outputLog) noexcept
            {
                T out { };
                deserialize<T>(parent, varName, outputLog, out);

                return out;
            }

            /**
             * Use if you want to deserialize data with sections "typename" and "value"
             * @param toDocument
             * @param parent
             * @param varName
             * @param value
             */
            template<typename T>
            static void deserialize(rapidjson::Value& parent, const std::uint64_t& index, std::string& outputLog,
                                    T& outputValue) noexcept
            {
                if (parent.Size() <= index)
                {
                    Serializer::formNotExistingMemberError(parent, index, outputLog);

                    return;
                }

                auto& self = parent[index];

                if (!self.HasMember("typename"))
                {
                    Serializer::formNotExistingMemberError(parent, "typename", outputLog);

                    return;
                }

                if (!self.HasMember("value"))
                {
                    Serializer::formNotExistingMemberError(parent, "value", outputLog);

                    return;
                }

                auto& typenameSection = self["typename"];
                auto& valueSection = self["value"];

                ValueDataView<T> outputValueData;
                outputValueData.m_parentValue = &parent;
                outputValueData.m_valueTypeName = &typenameSection;
                outputValueData.m_value = &valueSection;
                outputValueData.m_log = &outputLog;
                outputValueData.m_outputValue = &outputValue;

                deserializeUsing3rdPartyDynamicCasts<T>(outputValueData);
            }

            template<typename T>
            static T deserialize(rapidjson::Value& parent, const std::uint64_t& index, std::string& outputLog) noexcept
            {
                T out { };
                deserialize<T>(parent, index, outputLog, out);

                return out;
            }

            static void formNotExistingMemberError(const rapidjson::Value& parent, const std::string& varName,
                                                   std::string& outputLog) noexcept
            {
                // TODO: MAYBE PARENT NAME
                outputLog += "Error: member '" + varName + "' does not exist";
            }

            static void formNotExistingMemberError(const rapidjson::Value& parent, const std::uint64_t& index,
                                                   std::string& outputLog) noexcept
            {
                // TODO: MAYBE PARENT NAME
                outputLog += "Error: member with index '" + std::to_string(index) + "' does not exist";
            }

            template<typename T>
            static void
            addSections(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& typeName,
                        T& value) noexcept
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

                ValueDataView<T> outputValueData;
                outputValueData.m_document = &toDocument;
                outputValueData.m_parentKey = &valueK;
                outputValueData.m_parentValue = &parent;
                outputValueData.m_valueTypeName = &valueTypeV;
                outputValueData.m_value = &valueV;
                outputValueData.m_outputValue = &value;

                serializeUsing3rdPartyDynamicCasts(outputValueData);

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
                using type = T;
                // using type = std::remove_const_t<std::remove_pointer_t<std::remove_reference_t<T>>>;
            };

            template<typename T>
            using clear_type_t = clear_type_impl<T,
                    requires { typename T::element_type; }>::type;

            template<typename DerivedT, typename BaseT>
            static void deserializeBaseClass(ValueDataView<DerivedT>& valueData)
            {
                SerializerSpec<BaseT>::deserialize(valueData);
            }

            template<typename DerivedT, std::int64_t... Indices>
            static void deserializeBaseClassesImpl(ValueDataView<DerivedT>& valueData,
                                                   std::index_sequence<Indices...>)
            {
                (deserializeBaseClass<DerivedT, typename SerializerSpec<DerivedT>::template get_base_type<Indices>>(
                        valueData), ...);
            }

            template<typename DerivedT>
            static void deserializeBaseClasses(ValueDataView<DerivedT>& valueData)
            {
                if constexpr (requires { SerializerSpec<DerivedT>::base_classes_count&& SerializerSpec<DerivedT>::get_base_type; })
                {
                    deserializeBaseClassesImpl<DerivedT>(valueData,
                                                         std::make_index_sequence<SerializerSpec<DerivedT>::base_classes_count> {});
                }
            }

            template<typename BaseT, typename DerivedT>
            static void tryDeserializeAsDerived(ValueDataView<BaseT>& valueData)
            {
                ValueDataView<DerivedT> tmpData = valueData;

                if (SerializerSpec<DerivedT>::type_name == valueData.m_valueTypeName->GetString())
                {
                    /*assert(!outputValue && fmt::format(
                            "Some two or more serializer specializations contain the same value '{0}' for the type_name field."
                            " The class '{1}' cannot be deserialized correctly.", SerializerSpec<DerivedT>::type_name,
                            SerializerSpec<BaseT>::type_name));*/

                    // creating instance of derived
                    auto* tmp = new DerivedT();

                    tmpData.m_outputValue = tmp;

                    // deserializing all base parts of derived type (DerivedT)
                    deserializeBaseClasses<DerivedT>(tmpData);

                    // deserializing only values of derived type (DerivedT)
                    SerializerSpec<DerivedT>::deserialize(tmpData);

                    return;
                }

                // go ahead and try to deserialize as one of DerivedT derived classes
                tryDeserializeAsOneOfDerived<DerivedT>(tmpData);

                valueData.m_outputValue = tmpData.m_value;
            }

            template<typename BaseT, std::int64_t... Indices>
            static void tryDeserializeAsOneOfDerivedImpl(ValueDataView<BaseT>& valueData,
                                                         std::index_sequence<Indices...>)
            {
                (tryDeserializeAsDerived<BaseT, typename SerializerSpec<BaseT>::template get_derived_type<Indices>>(
                        valueData), ...);
            }

            template<typename BaseT>
            static void tryDeserializeAsOneOfDerived(ValueDataView<BaseT>& valueData)
            {
                if constexpr (requires { SerializerSpec<BaseT>::derived_classes_count&& SerializerSpec<BaseT>::get_derived_type; })
                {
                    tryDeserializeAsOneOfDerivedImpl<BaseT>(valueData,
                                                            std::make_index_sequence<SerializerSpec<BaseT>::derived_classes_count> {});
                }
            }

            template<typename BaseT, typename DerivedT>
            static void trySerializeDerivedClass(ValueDataView<BaseT>& valueData,
                                                 bool& isAnyOfDerivedSerialized)
            {
                const auto* derived = dynamic_cast<const DerivedT*>(valueData.m_outputValue);
                if (derived)
                {
                    // setting new typename
                    valueData.m_valueTypeName->SetString(SerializerSpec<DerivedT>::type_name.c_str(),
                                                         SerializerSpec<DerivedT>::type_name.length());

                    ValueDataView<DerivedT> tmpData = valueData;
                    tmpData.m_outputValue = derived;

                    // serializing all derived classes of DerivedT class
                    serializeDerivedClasses<DerivedT>(tmpData,
                                                      isAnyOfDerivedSerialized);

                    // serializing only data of DerivedT class
                    SerializerSpec<DerivedT>::serialize(tmpData);

                    isAnyOfDerivedSerialized = true;
                }
            }

            template<typename T, std::int64_t... Indices>
            static void serializeDerivedClassesImpl(ValueDataView<T>& valueData,
                                                    bool& isAnyOfDerivedSerialized,
                                                    std::index_sequence<Indices...>)
            {
                (trySerializeDerivedClass<T, typename SerializerSpec<T>::template get_derived_type<Indices>>(valueData,
                                                                                                             isAnyOfDerivedSerialized), ...);
            }

            template<typename T>
            static void serializeDerivedClasses(ValueDataView<T>& valueData,
                                                bool& isAnyOfDerivedSerialized)
            {
                if constexpr (requires { SerializerSpec<T>::derived_classes_count&& SerializerSpec<T>::get_derived_type; })
                {
                    serializeDerivedClassesImpl<T>(valueData,
                                                   isAnyOfDerivedSerialized,
                                                   std::make_index_sequence<SerializerSpec<T>::derived_classes_count> {});
                }
            }

            template<typename T, std::int64_t... Indices>
            static void serializeBaseClassesImpl(ValueDataView<T>& valueData,
                                                 std::index_sequence<Indices...>)
            {
                (serializeUsing3rdPartyDynamicCasts<typename SerializerSpec<T>::template get_base_type<Indices>>(
                        valueData),
                        ...);
            }

            template<typename T>
            static void serializeBaseClasses(ValueDataView<T>& valueData)
            {
                if constexpr (requires { SerializerSpec<T>::base_classes_count&& SerializerSpec<T>::get_base_type; })
                {
                    serializeBaseClassesImpl<T>(valueData,
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

            static void serialize(ValueDataView<T>& valueData) noexcept
            {
                valueData.m_value->SetInt(static_cast<std::int64_t>(*valueData.m_outputValue));
            }

            static void deserialize(ValueDataView<T>& valueData) noexcept
            {
                *valueData.m_outputValue = static_cast<T>(valueData.m_value->GetInt64());
            }
        };

        template<typename T>
        struct SerializerSpec<T*>
        {
            // using element_type = std::remove_const_t<std::remove_pointer_t<std::remove_reference_t<T>>>;

            static inline const rapidjson::Type rapidjson_type = SerializerSpec<T>::rapidjson_type;
            static inline const std::string type_name = SerializerSpec<T>::type_name;


            static void serialize(ValueDataView<T*>& dataView) noexcept
            {
                if (dataView.m_outputValue)
                {
                    ValueDataView<T> tmpView = dataView.template copyMeta<T>();
                    tmpView.m_outputValue = *dataView.m_outputValue;

                    SerializerSpec<T>::serialize(tmpView);
                }
                else
                {
                    *dataView.m_parentValue = rapidjson::Value(rapidjson::kNullType);
                }
            }

            static void deserialize(ValueDataView<T*>& dataView) noexcept
            {
                if (dataView.m_value->IsNull())
                {
                    dataView.m_outputValue = nullptr;
                    return;
                }

                ValueDataView<T> tmpView = dataView.template copyMeta<T>();
                //tmpView.m_outputValue = *dataView.m_outputValue;
                tmpView.m_outputValue = new T();
                // SerializerSpec<element_type>::deserializeDynamic(parent, value, typeName, outputLog, outputValue);
                SerializerSpec<T>::deserialize(tmpView);
                *dataView.m_outputValue = tmpView.m_outputValue;
                // dataView.m_outputValue = &tmpView.m_outputValue;
            }
        };

        template<typename T>
        struct SerializerSpec<std::shared_ptr<T>>
        {
            using element_type = T;

            static inline const rapidjson::Type rapidjson_type = SerializerSpec<element_type>::rapidjson_type;
            static inline const std::string type_name = SerializerSpec<element_type>::type_name;

            static void serialize(ValueDataView<std::shared_ptr<T>>& dataView) noexcept
            {
                if (dataView.m_outputValue)
                {
                    ValueDataView<T> tmpView = dataView.template copyMeta<T>();
                    tmpView.m_outputValue = &*dataView.m_outputValue;

                    SerializerSpec<T>::serialize(tmpView);
                }
                else
                {
                    *dataView.m_parentValue = rapidjson::Value(rapidjson::kNullType);
                }
            }

            static void deserialize(ValueDataView<std::shared_ptr<T>>& dataView) noexcept
            {
                if (dataView.m_value->IsNull())
                {
                    dataView.m_outputValue = nullptr;
                    return;
                }

                ValueDataView<T> tmpView = dataView.template copyMeta<T>();
                tmpView.m_outputValue = &*dataView.m_outputValue;

                // element_type* tmp = nullptr;
                SerializerSpec<T>::deserialize(tmpView);
                // outputValue = std::shared_ptr<T>(tmp);
            }
        };

        template<typename T>
        struct SerializerSpec<std::unique_ptr<T>>
        {
            using element_type = T;

            static inline const rapidjson::Type rapidjson_type = SerializerSpec<element_type>::rapidjson_type;
            static inline const std::string type_name = SerializerSpec<element_type>::type_name;

            static void serialize(ValueDataView<std::unique_ptr<T>>& dataView) noexcept
            {
                if (dataView.m_outputValue)
                {
                    ValueDataView<T> tmpView = dataView.template copyMeta<T>();
                    tmpView.m_outputValue = &*dataView.m_outputValue;

                    SerializerSpec<T>::serialize(tmpView);

                    SerializerSpec<element_type>::serialize(tmpView);
                }
                else
                {
                    *dataView.m_parentValue = rapidjson::Value(rapidjson::kNullType);
                }
            }

            static void deserialize(ValueDataView<std::unique_ptr<T>>& dataView) noexcept
            {
                if (dataView.m_value->IsNull())
                {
                    dataView.m_outputValue = nullptr;
                    return;
                }

                ValueDataView<T> tmpView = dataView.template copyMeta<T>();
                tmpView.m_outputValue = &*dataView.m_outputValue;

                // element_type* tmp = nullptr;
                SerializerSpec<T>::deserialize(tmpView);
                // outputValue = std::unique_ptr<element_type>(tmp);
            }
        };

        template<>
        struct SerializerSpec<char>
        {
            static inline const rapidjson::Type rapidjson_type = rapidjson::kStringType;
            static inline const std::string type_name = "char";


            static void serialize(ValueDataView<char>& dataView) noexcept
            {
                dataView.m_value->AddMember(*dataView.m_parentKey, *dataView.m_value, dataView.m_document->GetAllocator());
            }

            static void deserialize(ValueDataView<char>& dataView) noexcept
            {
                *dataView.m_outputValue = dataView.m_value->GetStringLength() > 0 ? dataView.m_value->GetString()[0] : ' ';
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

            static void deserialize(const rapidjson::Value& parent,
                                    const rapidjson::Value& value,
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

            static void deserialize(const rapidjson::Value& parent,
                                    const rapidjson::Value& value,
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

            static void serialize(ValueDataView<int32_t>& dataView) noexcept
            {
                dataView.m_value->SetInt(*dataView.m_outputValue);
            }

            static void deserialize(ValueDataView<int32_t>& dataView) noexcept
            {
                *dataView.m_outputValue = dataView.m_value->GetInt();
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

            static void deserialize(const rapidjson::Value& parent,
                                    const rapidjson::Value& value,
                                    const std::string& typeName,
                                    std::string& outputLog,
                                    std::int64_t& outputValue) noexcept
            {

            }
        };

        template<>
        struct SerializerSpec<float>
        {
            static inline const rapidjson::Type rapidjson_type = rapidjson::kNumberType;
            static inline const std::string type_name = "float";

            static void serialize(ValueDataView<float>& dataView) noexcept
            {
                dataView.m_value->SetFloat(*dataView.m_outputValue);
            }

            static void deserialize(ValueDataView<float>& dataView) noexcept
            {
                *dataView.m_outputValue = dataView.m_value->GetFloat();
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
}

#endif //SUNGEARENGINE_SERIALIZER_H
