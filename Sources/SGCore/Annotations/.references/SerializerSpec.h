template<>
struct SGCore::SerializerSpec<decltype(${currentNamespace}$::${currentStruct}$::${varWithoutGetterName}$)>
{
    using var_t = decltype(${currentNamespace}$::${currentStruct}$::${varWithoutGetterName}$);
    
    static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName, const var_t& value) noexcept;
};