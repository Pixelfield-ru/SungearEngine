//
// Created by stuka on 20.10.2024.
//

#ifndef SUNGEARENGINE_JSON_SERIALIZERIMPL_H
#define SUNGEARENGINE_JSON_SERIALIZERIMPL_H

#include "SGCore/Serde/Common.h"


template<typename... SharedDataT, typename T>
std::string SerializerImpl<FormatType::JSON>::to(const T& value,
                                                 SharedDataT&& ... sharedData) noexcept
{
    rapidjson::Document toDocument;
    toDocument.SetObject();

    // adding version of document
    toDocument.AddMember("version", "1", toDocument.GetAllocator());

    // adding type name of T
    rapidjson::Value typeNameSectionValue(rapidjson::kStringType);
    typeNameSectionValue.SetString(SerdeSpec<T, FormatType::JSON>::type_name.c_str(),
                                   SerdeSpec<T, FormatType::JSON>::type_name.length());

    // creating section that will contain all members of T
    rapidjson::Value valueSectionValue(rapidjson::kObjectType);

    // ==== serialization code

    // creating view of value with format pointers
    SerializableValueView<T, FormatType::JSON> valueView {};
    valueView.m_data = &value;
    valueView.m_version = "1";
    valueView.getValueContainer().m_document = &toDocument;
    valueView.getValueContainer().m_thisValue = &valueSectionValue;
    valueView.getValueContainer().m_typeNameValue = &typeNameSectionValue;

    // serializing value with attempt at dynamic casts to derived types
    Serializer::serializeWithDynamicChecks<T, FormatType::JSON, SharedDataT...>(valueView,
                                                                                std::forward<SharedDataT>(
                                                                                        sharedData
                                                                                )...
    );

    // =======================

    // adding type name of T after serializing (because type name can be changed)
    toDocument.AddMember("typeName", typeNameSectionValue, toDocument.GetAllocator());

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

    // trying to get 'version' member from document
    if(!document.HasMember("version"))
    {
        outputLog = "Error: Broken JSON document: root member 'version' does not exist.\n";
        return;
    }

    // getting 'version' member from document
    const std::string version = document["version"].GetString();

    // trying to get 'typeName' member from document
    if(!document.HasMember("typeName"))
    {
        outputLog = "Error: Broken JSON document: root member 'typeName' does not exist.\n";
        return;
    }

    // getting 'typeName' member from document
    const std::string typeName = document["typeName"].GetString();

    // trying to get 'value' member from document
    if(!document.HasMember("value"))
    {
        outputLog = "Error: Broken JSON document: root member 'value' does not exist.\n";
        return;
    }

    // getting 'value' member from document
    auto& jsonValue = document["value"];

    DeserializableValueView<T, FormatType::JSON> valueView;
    valueView.m_data = &outValue;
    valueView.m_version = version;
    valueView.getValueContainer().m_document = &document;
    valueView.getValueContainer().m_thisValue = &jsonValue;
    valueView.getValueContainer().m_typeName = typeName;
    valueView.getValueContainer().m_outputLog = &outputLog;

    Serializer::deserializeWithDynamicChecks<T, FormatType::JSON>(valueView,
                                                                  std::forward<SharedDataT>(
                                                                          sharedData
                                                                  )...
    );
}

#endif // SUNGEARENGINE_JSON_SERIALIZERIMPL_H
