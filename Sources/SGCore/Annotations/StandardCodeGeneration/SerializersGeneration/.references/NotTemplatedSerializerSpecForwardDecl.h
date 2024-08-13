template<>
struct SGCore::SerializerSpec<${structureName}$>
{
    static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, const ${structureName}$& value) noexcept;
};