//
// Created by stuka on 20.10.2024.
//

#ifndef SUNGEARENGINE_JSON_SERIALIZERIMPL_H
#define SUNGEARENGINE_JSON_SERIALIZERIMPL_H

#include "SGCore/Serde/Common.h"

template<typename T, typename... SharedDataT>
std::string SerializerImpl<FormatType::JSON>::to(const T& value,
                                                 SharedDataT&& ... sharedData) noexcept
{
    using clean_t = leave_pointers_t<T, false>;
    using value_view_t = leave_pointers_t<T, true>;

    rapidjson::Document toDocument;
    toDocument.SetObject();

    // adding version of document
    toDocument.AddMember(Detail::s_versionFieldName, "1", toDocument.GetAllocator());

    // adding type name of T
    std::string valueTypeName = Detail::getTypeName<clean_t, FormatType::JSON>();
    rapidjson::Value typeNameSectionValue(rapidjson::kStringType);
    typeNameSectionValue.SetString(valueTypeName.c_str(),
                                   valueTypeName.length(),
                                   toDocument.GetAllocator());

    // creating section that will contain all members of T
    rapidjson::Value valueSectionValue(rapidjson::kObjectType);

    // ==== serialization code

    // creating view of value with format pointers
    SerializableValueView<const value_view_t, FormatType::JSON> valueView {};
    valueView.m_data = &value;
    valueView.m_version = "1";
    valueView.container().m_document = &toDocument;
    valueView.container().m_thisValue = &toDocument;
    valueView.container().m_typeNameValue = &typeNameSectionValue;

    // serializing value with attempt at dynamic casts to derived types
    Serializer::serializeWithDynamicChecks<value_view_t, FormatType::JSON, SharedDataT...>(
        valueView,
        false,
        std::forward<SharedDataT>(
            sharedData
        )...
    );

    // =======================

    if(valueView.isDiscarded())
    {
        return "{\n}";
    }

    if constexpr(Detail::has_type_name<T, FormatType::JSON>)
    {
        // if type name was changed after serialization with dynamic checks (it means that type was serialized as derived type)
        // then we are adding type name field
        if(valueTypeName != valueView.container().m_typeNameValue->GetString())
        {
            if(toDocument.IsObject())
            {
                // adding type name of T after serializing (because type name can be changed)
                toDocument.AddMember(Detail::s_typeNameFieldName, typeNameSectionValue, toDocument.GetAllocator());
            }
            else if(toDocument.IsArray())
            {
                rapidjson::Value typeNameElement(rapidjson::kObjectType);
                typeNameElement.AddMember(Detail::s_arrayTypeNameFieldName, typeNameSectionValue, toDocument.GetAllocator());
                toDocument.PushBack(typeNameElement, toDocument.GetAllocator());
            }
        }
    }

    // converting to string
    rapidjson::StringBuffer stringBuffer;
    stringBuffer.Clear();
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);
    toDocument.Accept(writer);

    return stringBuffer.GetString();
}

/**
 * Converts JSON document to object.
 * @tparam T
 * @param toDocument
 * @param value
 * @return
 */
template<typename T, typename... SharedDataT>
void SerializerImpl<FormatType::JSON>::from(const std::string& formattedText,
                                            std::string& outputLog,
                                            T& outValue,
                                            SharedDataT&& ... sharedData) noexcept
{
    rapidjson::Document document;
    document.Parse(formattedText.c_str());

    // getting 'version' member from document
    std::string version = "1";
    // trying to get 'version' member from document
    if(document.IsObject() && document.HasMember(Detail::s_versionFieldName))
    {
        version = document[Detail::s_versionFieldName].GetString();
    }

    // getting 'typeName' member from document (if has)
    std::string typeName;
    if(document.IsObject() && document.HasMember(Detail::s_typeNameFieldName))
    {
        typeName = document[Detail::s_typeNameFieldName].GetString();
    }
    else if(document.IsArray() && !document.Empty() && document[document.Size() - 1].IsObject() && document[document.Size() - 1].HasMember(Detail::s_arrayTypeNameFieldName))
    {
        typeName = document[document.Size() - 1][Detail::s_arrayTypeNameFieldName].GetString();
    }

    DeserializableValueView<T, FormatType::JSON> valueView;
    valueView.m_data = &outValue;
    valueView.m_version = version;
    valueView.container().m_document = &document;
    valueView.container().m_thisValue = &document;
    valueView.container().m_typeName = typeName;
    valueView.container().m_outputLog = &outputLog;

    Serializer::deserializeWithDynamicChecks<T, FormatType::JSON>(valueView,
                                                                  false,
                                                                  std::forward<SharedDataT>(
                                                                          sharedData
                                                                  )...
    );
}

#endif // SUNGEARENGINE_JSON_SERIALIZERIMPL_H
