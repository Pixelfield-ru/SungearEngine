#pragma once

#include <SGCore/Serde/Serde.h>
#include <SGCore/Serde/StandardSerdeSpecs.h>

## for struct in structs

#include "{{ struct.filePath }}"

template<{{struct.templates.place(separator: ", ") }}SGCore::Serde::FormatType TFormatType>
struct SGCore::Serde::SerdeSpec<{{ struct.fullNameWithTemplates }}, TFormatType> : SGCore::Serde::BaseTypes<{{ struct.baseTypes.place(separator: ", ") }}>,
    SGCore::Serde::DerivedTypes<{{ struct.derivedTypes.place(separator: ", ") }}>
{
    static inline const std::string type_name = "{{ struct.fullName }}";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<{{ struct.fullNameWithTemplates }}, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<{{ struct.fullNameWithTemplates }}, TFormatType>& valueView) noexcept;
}

## endfor

// implementations

## for struct in structs

template<{{ struct.templates.place(separator: ", ") }}SGCore::Serde::FormatType TFormatType>
void SGCore::Serde::SerdeSpec<{{ struct.fullNameWithTemplates }}, TFormatType>::serialize(SGCore::Serde::SerializableValueView<{{ struct.fullNameWithTemplates }}, TFormatType>& valueView) noexcept
{
    ## for member in struct.members

    valueView.getValueContainer().addMember("{{ member.name }}", valueView.m_data->{{ member.getter }});

    ## endfor
}

template<{{ struct.templates.place(separator: ", ") }}SGCore::Serde::FormatType TFormatType>
void SGCore::Serde::SerdeSpec<{{ struct.fullNameWithTemplates }}, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<{{ struct.fullNameWithTemplates }}, TFormatType>& valueView) noexcept
{
    ## for member in struct.members

    const auto {{ member.name }} = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->{{ member.getter }})>>>("{{ member.struct.name }}");
    if({{ member.name }})
    {
        ## if member.hasSetter
        valueView.m_data->{{ member.setter }}(*{{ member.name }});
        ## else
        valueView.m_data->{{ member.name }} = *{{ member.name }};
        ## endif
    }

    ## endfor
}

## endfor