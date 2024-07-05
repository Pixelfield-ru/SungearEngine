template<${structureTemplateArgs}$>
void SGCore::SerializerSpec<${structureName}$<${structureTemplateNames}$>>::serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, const ${structureName}$<${structureTemplateNames}$>& value) noexcept
{
    rapidjson::Value k(rapidjson::kObjectType);
    rapidjson::Value v(rapidjson::kObjectType);

    k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

${valuesAddCode}$
        
        parent.AddMember(k, v, toDocument.GetAllocator());
}