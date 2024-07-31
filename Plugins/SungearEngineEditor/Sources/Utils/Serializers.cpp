//
// Created by Ilya on 31.07.2024.
//

#include "Serializers.h"

// TEMPLATE CODE
// TODO: MUST BE USED IN SERIALIZERS GENERATION
void SGCore::SerializerSpec<SGE::Toolchain>::serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                                                       const std::string& varName, const SGE::Toolchain& value) noexcept
{
    auto* dyn0 = dynamic_cast<const SGE::VisualStudioToolchain*>(&value);
    if(dyn0)
    {
        Serializer::serialize(toDocument, parent, varName, dyn0);
        return;
    }

    rapidjson::Value k(rapidjson::kStringType);
    rapidjson::Value v(rapidjson::kObjectType);

    k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

    rapidjson::Value valueTypeK(rapidjson::kStringType);
    rapidjson::Value valueTypeV(rapidjson::kObjectType);

    Serializer::serialize(toDocument, v, "typename", "SGE::Toolchain");
    const std::string valueTypeName = "SGE::Toolchain";
    valueTypeK.SetString(valueTypeName.c_str(), valueTypeName.length(), toDocument.GetAllocator());
    v.AddMember(valueTypeK, valueTypeV, toDocument.GetAllocator());

    rapidjson::Value valueK(rapidjson::kStringType);
    rapidjson::Value valueV(rapidjson::kObjectType);

    valueK.SetString("value", 5, toDocument.GetAllocator());
    serializeOnlyValueType(toDocument, valueV, value);
    v.AddMember(valueTypeK, valueTypeV, toDocument.GetAllocator());

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

void SGCore::SerializerSpec<SGE::Toolchain>::serializeOnlyValueType(rapidjson::Document& toDocument,
                                                                    rapidjson::Value& parent,
                                                                    const SGE::Toolchain& value) noexcept
{

}
