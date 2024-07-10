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
                                                                  const std::string& varName) noexcept
{
    /*auto& self = parent[varName.c_str()];

    AnnotationsProcessor value;
    value.m_supportedAnnotations = Serializer::deserialize<std::unordered_map<std::string, AnnotationsProcessor::Annotation>>(self, "m_supportedAnnotations");
    value.m_currentAnnotations = Serializer::deserialize<std::vector<AnnotationsProcessor::Annotation>>(self, "m_currentAnnotations");

    return value;*/

    return { };
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
                                                                                 const std::string& varName) noexcept
{
    /*auto& self = parent[varName.c_str()];

    AnnotationsProcessor::AnnotationArg value;
    value.m_name = Serializer::deserialize<std::string>(self, "m_name");
    value.m_isUnnecessary = Serializer::deserialize<bool>(self, "m_isUnnecessary");
    value.m_supportedAnnotations = Serializer::deserialize<std::unordered_map<std::string, AnnotationsProcessor::Annotation>>(self, "m_supportedAnnotations");
    value.m_currentAnnotations = Serializer::deserialize<std::vector<AnnotationsProcessor::Annotation>>(self, "m_currentAnnotations");

    return value;*/

    return { };
}