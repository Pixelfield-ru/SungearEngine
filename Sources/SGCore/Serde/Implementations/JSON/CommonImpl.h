//
// Created by stuka on 20.10.2024.
//

#ifndef SUNGEARENGINE_JSON_COMMONIMPL_H
#define SUNGEARENGINE_JSON_COMMONIMPL_H

#include "SGCore/Utils/TypeTraits.h"
#include "SGCore/Serde/Common.h"

namespace SGCore::Serde
{
    template<>
    struct FormatInfo<FormatType::JSON>
    {
        using member_iterator_t = rapidjson::Value::MemberIterator;
        using array_iterator_t = rapidjson::Value::ValueIterator;
    };

    template<>
    struct DeserializableValueContainer<FormatType::JSON>
    {
        template<FormatType>
        friend struct SerializerImpl;

        friend struct Serializer;

        template<typename, FormatType>
        friend struct DeserializableValueView;

        bool hasMember(const std::string& memberName) const noexcept
        {
            return m_document && m_thisValue && m_thisValue->HasMember(memberName.c_str());
        }

        template<typename T, typename... SharedDataT>
        std::optional<T> getMember(const std::string& memberName, SharedDataT&&... sharedData) noexcept
        {
            if(!(m_thisValue && m_document))
            {
                if(m_outputLog)
                {
                    *m_outputLog += "Error: Can not get member with name '" + memberName + "': m_thisValue or m_document is null.\n";
                }

                return std::nullopt;
            }

            if(!m_thisValue->HasMember(memberName.c_str()))
            {
                if(m_outputLog)
                {
                    *m_outputLog += "Error: Can not get member with name '" + memberName + "': this member does not exist.\n";
                }

                return std::nullopt;
            }

            // getting member
            auto& member = (*m_thisValue)[memberName.c_str()];

            // getting typeName section of member
            std::string typeName = getTypeName(member);

            T outputValue { };

            // creating value view of member
            DeserializableValueView<T, FormatType::JSON> valueView { };
            valueView.container().m_document = m_document;
            valueView.container().m_thisValue = &member;
            valueView.container().m_parent = this;
            valueView.container().m_outputLog = m_outputLog;
            valueView.container().m_typeName = typeName;
            valueView.m_data = &outputValue;

            // deserializing member with dynamic checks
            Serializer::deserializeWithDynamicChecks<T, FormatType::JSON>(valueView, std::forward<SharedDataT>(sharedData)...);

            return outputValue;
        }

        template<typename T, typename ContainerT = std::vector<T>, typename... SharedDataT>
        requires(requires(ContainerT con, T obj) { con.push_back(obj); })
        [[nodiscard]] ContainerT getAsArray(SharedDataT&&... sharedData) noexcept
        {
            if(!(m_thisValue && m_document))
            {
                if(m_outputLog)
                {
                    *m_outputLog += "Error: Can not get value as array: m_thisValue or m_document is null.\n";
                }

                return { };
            }

            if(!m_thisValue->IsArray())
            {
                if(m_outputLog)
                {
                    *m_outputLog += "Error: Can not get value as array: value is not an array.\n";
                }

                return { };
            }

            ContainerT outputValue;

            for(std::size_t i = 0; i < m_thisValue->Size(); ++i)
            {
                T tmpVal { };

                auto& elementValue = (*m_thisValue)[i];

                if(elementValue.IsObject() && elementValue.HasMember(Detail::s_arrayTypeNameFieldName))
                {
                    continue;
                }

                // getting 'typeName' section
                std::string typeNameSection = getTypeName(elementValue);

                // creating value view of member
                DeserializableValueView<T, FormatType::JSON> valueView { };
                valueView.container().m_document = m_document;
                valueView.container().m_thisValue = &elementValue;
                valueView.container().m_parent = this;
                valueView.container().m_outputLog = m_outputLog;
                valueView.container().m_typeName = typeNameSection;
                valueView.m_data = &tmpVal;

                Serializer::deserializeWithDynamicChecks<T, FormatType::JSON>(valueView, std::forward<SharedDataT>(sharedData)...);

                outputValue.push_back(std::move(tmpVal));
            }

            return outputValue;
        }

        [[nodiscard]] std::int64_t getAsInt64() const noexcept
        {
            if(!m_thisValue)
            {
                if(m_outputLog)
                {
                    *m_outputLog += "Error: Can not get value as int64: m_thisValue is null.\n";
                }

                return { };
            }

            return m_thisValue->GetInt64();
        }

        [[nodiscard]] float getAsFloat() const noexcept
        {
            if(!m_thisValue)
            {
                if(m_outputLog)
                {
                    *m_outputLog += "Error: Can not get value as float: m_thisValue is null.\n";
                }

                return { };
            }

            return m_thisValue->GetFloat();
        }

        template<typename CharT>
        [[nodiscard]] std::basic_string<CharT> getAsString() const noexcept
        {
            if(!m_thisValue)
            {
                if(m_outputLog)
                {
                    *m_outputLog += "Error: Can not get value as float: m_thisValue is null.\n";
                }

                return { };
            }

            return SGCore::Utils::template fromUTF8<CharT>(m_thisValue->GetString());
        }

        [[nodiscard]] bool getAsBool() const noexcept
        {
            if(!m_thisValue) return false;

            return m_thisValue->GetBool();
        }

        [[nodiscard]] rapidjson::Value::MemberIterator memberBegin() const noexcept
        {
            if(!m_thisValue) return { };

            return m_thisValue->MemberBegin();
        }

        [[nodiscard]] rapidjson::Value::MemberIterator memberEnd() const noexcept
        {
            if(!m_thisValue) return { };

            return m_thisValue->MemberEnd();
        }

        [[nodiscard]] rapidjson::Value::ValueIterator begin() const noexcept
        {
            if(!m_thisValue) return { };

            return m_thisValue->Begin();
        }

        [[nodiscard]] rapidjson::Value::ValueIterator end() const noexcept
        {
            if(!m_thisValue) return { };

            return m_thisValue->End();
        }

        template<typename T, typename... SharedDataT>
        std::optional<T> getMember(const rapidjson::Value::MemberIterator& memberIterator, SharedDataT&&... sharedData) noexcept
        {
            if(!(m_thisValue && m_document))
            {
                if(m_outputLog)
                {
                    *m_outputLog += "Error: Can not get member with name '" + std::string(memberIterator->name.GetString()) +
                                   "': m_thisValue or m_document is null.\n";
                }

                return std::nullopt;
            }

            // getting member
            auto& member = memberIterator->value;

            if(member.IsString() && (memberIterator->name == Detail::s_typeNameFieldName || memberIterator->name == Detail::s_versionFieldName))
            {
                return std::nullopt;
            }

            // getting typeName section of member
            std::string typeName = getTypeName(member);

            T outputValue { };

            // creating value view of member
            DeserializableValueView<T, FormatType::JSON> valueView { };
            valueView.container().m_document = m_document;
            valueView.container().m_thisValue = &member;
            valueView.container().m_parent = this;
            valueView.container().m_outputLog = m_outputLog;
            valueView.container().m_typeName = typeName;
            valueView.m_data = &outputValue;

            // deserializing member with dynamic checks
            Serializer::deserializeWithDynamicChecks<T, FormatType::JSON>(valueView, std::forward<SharedDataT>(sharedData)...);

            return outputValue;
        }

        template<typename T, typename... SharedDataT>
        std::optional<T> getMember(const rapidjson::Value::ValueIterator& arrayIterator, SharedDataT&&... sharedData) noexcept
        {
            if(!(m_thisValue && m_document))
            {
                if(m_outputLog)
                {
                    *m_outputLog += "Error: Can not get value from array: m_thisValue or m_document is null.\n";
                }

                return std::nullopt;
            }

            // getting member
            auto& member = *arrayIterator;

            if(member.IsObject() && member.HasMember(Detail::s_arrayTypeNameFieldName))
            {
                return std::nullopt;
            }

            // getting typeName section of member
            std::string typeName = getTypeName(member);

            T outputValue { };

            // creating value view of member
            DeserializableValueView<T, FormatType::JSON> valueView { };
            valueView.container().m_document = m_document;
            valueView.container().m_thisValue = &member;
            valueView.container().m_parent = this;
            valueView.container().m_outputLog = m_outputLog;
            valueView.container().m_typeName = typeName;
            valueView.m_data = &outputValue;

            // deserializing member with dynamic checks
            Serializer::deserializeWithDynamicChecks<T, FormatType::JSON>(valueView, std::forward<SharedDataT>(sharedData)...);

            return outputValue;
        }

        [[nodiscard]] std::string getMemberName(const rapidjson::Value::MemberIterator& memberIterator) const noexcept
        {
            return memberIterator->name.GetString();
        }

        [[nodiscard]] std::string getMemberTypeName(const rapidjson::Value::MemberIterator& memberIterator) const noexcept
        {
            return getTypeName(memberIterator->value);
        }

        [[nodiscard]] std::string getMemberTypeName(const rapidjson::Value::ValueIterator& memberIterator) const noexcept
        {
            return getTypeName((*memberIterator));
        }

        [[nodiscard]] bool isNull() const noexcept
        {
            return m_thisValue && m_thisValue->IsNull();
        }

        [[nodiscard]] bool hasTypeName() const noexcept
        {
            if(!m_thisValue) return false;

            if(m_thisValue->IsObject())
            {
                return m_thisValue->HasMember(Detail::s_typeNameFieldName);
            }

            if(m_thisValue->IsArray())
            {
                return !m_thisValue->Empty() && (*m_thisValue)[m_thisValue->Size() - 1].IsObject() && (*m_thisValue)[m_thisValue->Size() - 1].HasMember(Detail::s_arrayTypeNameFieldName);
            }

            return false;
        }

    private:
        static std::string getTypeName(const rapidjson::Value& from) noexcept
        {
            if(from.IsObject())
            {
                if(from.HasMember(Detail::s_typeNameFieldName))
                {
                    return from[Detail::s_typeNameFieldName].GetString();
                }
            }
            else if(from.IsArray())
            {
                // meta object with type name of array is always last in arrays
                if(!from.Empty() && from[from.Size() - 1].IsObject() && from[from.Size() - 1].HasMember(Detail::s_arrayTypeNameFieldName))
                {
                    return from[from.Size() - 1][Detail::s_arrayTypeNameFieldName].GetString();
                }
            }

            return "";
        }

        DeserializableValueContainer<FormatType::JSON>* m_parent { };
        rapidjson::Document* m_document { };
        rapidjson::Value* m_thisValue { };
        std::string* m_outputLog { };
        std::string m_typeName;
    };

    /**
     * Value container for JSON format.
     */
    template<>
    struct SerializableValueContainer<FormatType::JSON>
    {
        template<FormatType>
        friend struct SerializerImpl;

        friend struct Serializer;

        template<typename T0, FormatType TFormatType0>
        friend struct SerializableValueView;

        template<typename T, typename... SharedDataT>
        void addMember(const std::string& name, const T& value, SharedDataT&&... sharedData) noexcept
        {
            using clean_t = leave_pointers_t<T, false>;
            using value_view_t = leave_pointers_t<T, true>;

            if (!(m_thisValue && m_document)) return;

            // removing member with this name if it is already exists
            if (m_thisValue->IsObject() && m_thisValue->HasMember(name.c_str()))
            {
                m_thisValue->RemoveMember(name.c_str());
            }

            // creating key that contains passed name
            rapidjson::Value valueNameKey(rapidjson::kStringType);
            // creating value root section
            rapidjson::Value valueRootSection(rapidjson::kObjectType);

            // setting name
            valueNameKey.SetString(name.c_str(), name.length(), m_document->GetAllocator());

            // creating type name of T value
            std::string valueTypeName = Detail::getTypeName<clean_t, FormatType::JSON>();
            rapidjson::Value typeNameSectionValue(rapidjson::kStringType);
            typeNameSectionValue.SetString(valueTypeName.c_str(),
                                           valueTypeName.length(),
                                           m_document->GetAllocator());

            // creating section that will contain all members of T0
            rapidjson::Value valueSectionValue(rapidjson::kObjectType);

            // ==== value serialization code

            // creating view of value with format pointers
            SerializableValueView<const value_view_t, FormatType::JSON> valueView {};
            valueView.m_data = &value;
            valueView.container().m_document = m_document;
            valueView.container().m_thisValue = &valueRootSection;
            valueView.container().m_typeNameValue = &typeNameSectionValue;
            valueView.container().m_parent = this;

            // serializing value with attempt at dynamic casts to derived types
            Serializer::serializeWithDynamicChecks<value_view_t, FormatType::JSON>(valueView, false, std::forward<SharedDataT>(sharedData)...);

            // =======================

            if(!valueView.isDiscarded())
            {
                // if type name was changed after serialization with dynamic checks (it means that type was serialized as derived type)
                // then we are adding type name field
                if(valueTypeName != valueView.container().m_typeNameValue->GetString())
                {
                    // adding typeName section
                    if(valueRootSection.IsObject())
                    {
                        valueRootSection.AddMember(Detail::s_typeNameFieldName, typeNameSectionValue, m_document->GetAllocator());
                    }
                    else if(valueRootSection.IsArray())
                    {
                        rapidjson::Value typeNameElement(rapidjson::kObjectType);
                        typeNameElement.AddMember(Detail::s_arrayTypeNameFieldName, typeNameSectionValue, m_document->GetAllocator());
                        valueRootSection.PushBack(typeNameElement, m_document->GetAllocator());
                    }
                }

                if(m_thisValue->IsArray())
                {
                    m_thisValue->PushBack(valueRootSection, m_document->GetAllocator());

                    return;
                }

                // adding value section to document
                m_thisValue->AddMember(valueNameKey, valueRootSection, m_document->GetAllocator());
            }
        }

        template<typename T, typename... SharedDataT>
        void pushBack(const T& value, SharedDataT&&... sharedData) noexcept
        {
            addMember<T, SharedDataT...>("", value, std::forward<SharedDataT>(sharedData)...);
        }

        void setAsNull() noexcept
        {
            if(!m_thisValue) return;

            m_thisValue->SetNull();
        }

        void setAsFloat(const float& f) noexcept
        {
            if(!m_thisValue) return;

            m_thisValue->SetFloat(f);
        }

        void setAsInt64(const std::int64_t& i) noexcept
        {
            if(!m_thisValue) return;

            m_thisValue->SetInt64(i);
        }

        template<typename CharT>
        void setAsString(const std::basic_string<CharT>& str) noexcept
        {
            if(!(m_thisValue && m_document)) return;

            const std::string utf8String = SGCore::Utils::toUTF8(str);

            m_thisValue->SetString(utf8String.c_str(), utf8String.length(), m_document->GetAllocator());
        }

        void setAsArray() noexcept
        {
            if(!m_thisValue) return;

            m_thisValue->SetArray();
        }

        void setAsBool(bool b) noexcept
        {
            if(!m_thisValue) return;

            m_thisValue->SetBool(b);
        }

        void setTypeName(const std::string& typeName) noexcept
        {
            if(!(m_typeNameValue && m_document)) return;

            m_typeNameValue->SetString(typeName.c_str(), typeName.length(), m_document->GetAllocator());
        }

        [[nodiscard]] std::string getTypeName(const std::string& typeName) const noexcept
        {
            if(!m_typeNameValue) return "";

            return m_typeNameValue->GetString();
        }

        auto& getParent() noexcept
        {
            return *m_parent;
        }

    private:
        SerializableValueContainer<FormatType::JSON>* m_parent { };
        rapidjson::Document* m_document { };
        rapidjson::Value* m_thisValue { };
        rapidjson::Value* m_typeNameValue { };
    };
}

#endif // SUNGEARENGINE_JSON_COMMONIMPL_H
