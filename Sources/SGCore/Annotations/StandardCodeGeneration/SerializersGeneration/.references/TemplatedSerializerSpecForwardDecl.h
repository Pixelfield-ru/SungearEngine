template<${structureTemplateArgs}$>
struct SGCore::SerializerSpec<${structureName}$<${structureTemplateNames}$>>
{
    static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, const ${structureName}$<${structureTemplateNames}$>& value) noexcept;
};