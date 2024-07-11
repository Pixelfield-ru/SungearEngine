//
// Created by ilya on 02.07.24.
//

#include "Serializer.h"

// ===============================================================================================================================
// ===============================================================================================================================
// ===============================================================================================================================
// STANDARD SERIALIZABLES FOR TYPES


/*template<> void SGCore::SerializerSpec<std::int8_t>::serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                                                    const std::string& varName, const std::int8_t& value) noexcept
{
    parent.AddMember(rapidjson::StringRef(varName.c_str()), value, toDocument.GetAllocator());
}*/

void SGCore::SerializerSpec<SGCore::AnnotationsProcessor>::serialize(rapidjson::Document& toDocument,
                                                                     rapidjson::Value& parent,
                                                                     const std::string& varName,
                                                                     const AnnotationsProcessor& value) noexcept
{
    rapidjson::Value k(rapidjson::kStringType);
    rapidjson::Value v(rapidjson::kObjectType);

    k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

    SGCore::Serializer::serialize(toDocument, v, "m_supportedAnnotations", value.getSupportedAnnotations());
    SGCore::Serializer::serialize(toDocument, v, "m_currentAnnotations", value.getAnnotations());

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

SGCore::AnnotationsProcessor
SGCore::SerializerSpec<SGCore::AnnotationsProcessor>::deserialize(const rapidjson::Value& parent,
                                                                  const rapidjson::Value& value,
                                                                  std::string& outputLog) noexcept
{
    AnnotationsProcessor outputValue;
    outputValue.m_supportedAnnotations =
            Serializer::deserialize<std::unordered_map<std::string, AnnotationsProcessor::Annotation>>(value,
                                                                                                       "m_supportedAnnotations",
                                                                                                       outputLog);
    outputValue.m_currentAnnotations = Serializer::deserialize<std::vector<AnnotationsProcessor::Annotation>>(value,
                                                                                                              "m_currentAnnotations",
                                                                                                              outputLog);

    return outputValue;
}

void
SGCore::SerializerSpec<SGCore::AnnotationsProcessor::Member>::serialize(rapidjson::Document& toDocument,
                                                                        rapidjson::Value& parent,
                                                                        const std::string& varName,
                                                                        const AnnotationsProcessor::Member& value) noexcept
{
    rapidjson::Value k(rapidjson::kStringType);
    rapidjson::Value v(rapidjson::kObjectType);

    k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

    SGCore::Serializer::serialize(toDocument, v, "m_name", value.m_name);
    SGCore::Serializer::serialize(toDocument, v, "m_annotations", value.m_annotations);

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

SGCore::AnnotationsProcessor::Member
SGCore::SerializerSpec<SGCore::AnnotationsProcessor::Member>::deserialize(const rapidjson::Value& parent,
                                                                          const rapidjson::Value& value,
                                                                          std::string& outputLog) noexcept
{
    AnnotationsProcessor::Member outputValue;
    outputValue.m_name = Serializer::deserialize<std::string>(value, "m_name", outputLog);
    outputValue.m_annotations =
            Serializer::deserialize<std::unordered_map<std::string, AnnotationsProcessor::Annotation>>(value,
                                                                                                       "m_annotations",
                                                                                                       outputLog);

    return outputValue;
}

void SGCore::SerializerSpec<SGCore::AnnotationsProcessor::Annotation>::serialize(rapidjson::Document& toDocument,
                                                                                 rapidjson::Value& parent,
                                                                                 const std::string& varName,
                                                                                 const AnnotationsProcessor::Annotation& value) noexcept
{
    rapidjson::Value k(rapidjson::kStringType);
    rapidjson::Value v(rapidjson::kObjectType);

    k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

    SGCore::Serializer::serialize(toDocument, v, "m_name", value.m_name);
    SGCore::Serializer::serialize(toDocument, v, "m_filePath", value.m_filePath);
    SGCore::Serializer::serialize(toDocument, v, "m_acceptableArgs", value.m_acceptableArgs);
    SGCore::Serializer::serialize(toDocument, v, "m_currentArgs", value.m_currentArgs);
    SGCore::Serializer::serialize(toDocument, v, "m_anonymousArgs", value.m_anonymousArgs);

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

SGCore::AnnotationsProcessor::Annotation
SGCore::SerializerSpec<SGCore::AnnotationsProcessor::Annotation>::deserialize(const rapidjson::Value& parent,
                                                                              const rapidjson::Value& value,
                                                                              std::string& outputLog) noexcept
{
    AnnotationsProcessor::Annotation outputValue;
    outputValue.m_name = Serializer::deserialize<std::string>(value, "m_name", outputLog);
    outputValue.m_filePath = Serializer::deserialize<std::filesystem::path>(value, "m_filePath", outputLog);
    outputValue.m_acceptableArgs =
            Serializer::deserialize<std::unordered_map<std::string, AnnotationsProcessor::AnnotationArg>>(value,
                                                                                                          "m_acceptableArgs",
                                                                                                          outputLog);
    outputValue.m_currentArgs =
            Serializer::deserialize<std::unordered_map<std::string, AnnotationsProcessor::AnnotationArg>>(value,
                                                                                                          "m_currentArgs",
                                                                                                          outputLog);

    outputValue.m_anonymousArgs =
            Serializer::deserialize<std::vector<AnnotationsProcessor::AnnotationArg>>(value,
                                                                                      "m_anonymousArgs",
                                                                                      outputLog);

    return outputValue;
}

void SGCore::SerializerSpec<SGCore::AnnotationsProcessor::AnnotationArg>::serialize(rapidjson::Document& toDocument,
                                                                                    rapidjson::Value& parent,
                                                                                    const std::string& varName,
                                                                                    const AnnotationsProcessor::AnnotationArg& value) noexcept
{
    rapidjson::Value k(rapidjson::kStringType);
    rapidjson::Value v(rapidjson::kObjectType);

    k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

    SGCore::Serializer::serialize(toDocument, v, "m_name", value.m_name);
    SGCore::Serializer::serialize(toDocument, v, "m_isUnnecessary", value.m_isUnnecessary);
    SGCore::Serializer::serialize(toDocument, v, "m_requiredValuesCount", value.m_requiredValuesCount);

    rapidjson::Value valuesK(rapidjson::kStringType);
    rapidjson::Value valuesV(rapidjson::kArrayType);

    const std::string valuesKeyName = "m_values";

    valuesK.SetString(valuesKeyName.c_str(), valuesKeyName.length(), toDocument.GetAllocator());

    for (const auto& val: value.m_values)
    {
        rapidjson::Value anyV(rapidjson::kObjectType);

        auto* asStr = std::any_cast<std::string>(&val);
        auto* asAnnotation = std::any_cast<AnnotationsProcessor::Annotation>(&val);

        if (asStr)
        {
            SGCore::Serializer::serialize(toDocument, anyV, "value", *asStr);
            anyV.AddMember("anyType", "std::string", toDocument.GetAllocator());
        }
        else if (asAnnotation)
        {
            SGCore::Serializer::serialize(toDocument, anyV, "value", *asAnnotation);
            anyV.AddMember("anyType", "SGCore::AnnotationsProcessor::Annotation", toDocument.GetAllocator());
        }

        valuesV.PushBack(anyV, toDocument.GetAllocator());
    }

    v.AddMember(valuesK, valuesV, toDocument.GetAllocator());

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

SGCore::AnnotationsProcessor::AnnotationArg
SGCore::SerializerSpec<SGCore::AnnotationsProcessor::AnnotationArg>::deserialize(const rapidjson::Value& parent,
                                                                                 const rapidjson::Value& value,
                                                                                 std::string& outputLog) noexcept
{
    AnnotationsProcessor::AnnotationArg outputValue;
    outputValue.m_name = Serializer::deserialize<std::string>(value, "m_name", outputLog);
    outputValue.m_isUnnecessary = Serializer::deserialize<bool>(value, "m_isUnnecessary", outputLog);
    outputValue.m_requiredValuesCount = Serializer::deserialize<std::int64_t>(value, "m_requiredValuesCount", outputLog);

    auto& valuesMember = value["m_values"];

    for (rapidjson::Value::ConstMemberIterator iter = valuesMember.MemberBegin(); iter != valuesMember.MemberEnd(); ++iter)
    {
        // std::string anyType = iter->value["anyType"].GetString();

        /*if(anyType == "std::string")
        {
            outputValue.m_values.emplace_back(SerializerSpec<std::string>::deserialize(iter->value, iter->value["value"], outputLog));
        }
        else if(anyType == "SGCore::AnnotationsProcessor::Annotation")
        {
            outputValue.m_values.emplace_back(SerializerSpec<AnnotationsProcessor::Annotation>::deserialize(iter->value, iter->value["value"], outputLog));
        }*/
    }

    return outputValue;
}