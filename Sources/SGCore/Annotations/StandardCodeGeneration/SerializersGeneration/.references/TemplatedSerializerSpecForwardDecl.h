template<${structureTemplateArgs}$, SGCore::Serde::FormatType TFormatType>
struct SGCore::Serde::SerdeSpec<${structureName}$<${structureTemplateNames}$>, TFormatType> : SGCore::Serde::BaseTypes<${structureBaseTypes}$>,
        SGCore::Serde::DerivedTypes<${structureDerivedTypes}$>
{
    static inline const std::string type_name = "${structureName}$";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<${structureName}$<${structureTemplateNames}$>, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<${structureName}$<${structureTemplateNames}$>, TFormatType>& valueView) noexcept;
};