template<${structureTemplateArgs}$, SGCore::Serde::FormatType TFormatType>
void SGCore::Serde::SerdeSpec<${structureName}$<${structureTemplateNames}$>, TFormatType>::serialize(SGCore::Serde::SerializableValueView<${structureName}$<${structureTemplateNames}$>, TFormatType>& valueView) noexcept
{
${valuesSerializeCode}$
}

template<${structureTemplateArgs}$, SGCore::Serde::FormatType TFormatType>
void SGCore::Serde::SerdeSpec<${structureName}$<${structureTemplateNames}$>, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<${structureName}$<${structureTemplateNames}$>, TFormatType>& valueView) noexcept
{
${valuesDeserializeCode}$
}