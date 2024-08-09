template<${structureTemplateArgs}$>
void SGCore::SerializerSpec<${structureName}$<${structureTemplateNames}$>>::serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, const ${structureName}$<${structureTemplateNames}$>& value) noexcept
{
    rapidjson::Value k(rapidjson::kObjectType);
    rapidjson::Value v(rapidjson::kObjectType);

    k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

${valuesAddCode}$

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