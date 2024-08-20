template<SGCore::Serde::FormatType TFormatType>
void SGCore::Serde::SerdeSpec<${structureName}$, TFormatType>::serialize(SGCore::Serde::SerializableValueView<${structureName}$, TFormatType>& valueView) noexcept
{
${valuesSerializeCode}$
}

template<SGCore::Serde::FormatType TFormatType>
void SGCore::Serde::SerdeSpec<${structureName}$, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<${structureName}$, TFormatType>& valueView) noexcept
{
${valuesDeserializeCode}$
}