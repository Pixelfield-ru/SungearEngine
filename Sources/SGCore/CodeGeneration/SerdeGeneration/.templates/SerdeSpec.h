#pragma once

#include <SGCore/Serde/Serde.h>
#include <SGCore/Serde/StandardSerdeSpecs.h>

## for struct in structs

#include "{{ struct.filePath }}"

// SERDE FORWARD DECL FOR struct '{{ struct.fullName }}'
// =================================================================================
template<
## if struct.hasMember(name: "templateDecl")
        {{ struct.templateDecl }},
## endif
        SGCore::Serde::FormatType TFormatType
        >
struct SGCore::Serde::SerdeSpec<{{ struct.fullNameWithTemplate }}, TFormatType> : SGCore::Serde::BaseTypes<{{ struct.baseTypes.place(separator: ", ") }}>,
    SGCore::Serde::DerivedTypes<{{ struct.derivedTypes.place(separator: ", ") }}>
{
    static inline const std::string type_name = "{{ struct.fullName }}";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<{{ struct.fullNameWithTemplate }}, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<{{ struct.fullNameWithTemplate }}, TFormatType>& valueView) noexcept;
};
// =================================================================================

## endfor

// implementations

## for struct in structs

// SERDE IMPL FOR struct '{{ struct.fullName }}'
// =================================================================================
template<
## if struct.hasMember(name: "templateDecl")
        {{ struct.templateDecl }},
## endif
        SGCore::Serde::FormatType TFormatType
        >
void SGCore::Serde::SerdeSpec<{{ struct.fullNameWithTemplate }}, TFormatType>::serialize(SGCore::Serde::SerializableValueView<{{ struct.fullNameWithTemplate }}, TFormatType>& valueView) noexcept
{
    ## for member in struct.members

    ## if member.hasMember(name: "getter")
    valueView.getValueContainer().addMember("{{ member.name }}", valueView.m_data->{{ member.getter }});
    ## else
    valueView.getValueContainer().addMember("{{ member.name }}", valueView.m_data->{{ member.name }});
    ## endif

    ## endfor
}

template<
## if struct.hasMember(name: "templateDecl")
        {{ struct.templateDecl }},
## endif
        SGCore::Serde::FormatType TFormatType
        >
void SGCore::Serde::SerdeSpec<{{ struct.fullNameWithTemplate }}, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<{{ struct.fullNameWithTemplate }}, TFormatType>& valueView) noexcept
{
    ## for member in struct.members

    ## if member.hasMember(name: "getter")
    const auto {{ member.name }} = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->{{ member.getter }})>>>("{{ member.name }}");
    ## else
    const auto {{ member.name }} = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->{{ member.name }})>>>("{{ member.name }}");
    ## endif

    if({{ member.name }})
    {
        ## if member.hasMember(name: "setter")
        valueView.m_data->{{ member.setter }}(*{{ member.name }});
        ## else
        valueView.m_data->{{ member.name }} = *{{ member.name }};
        ## endif
    }

    ## endfor
}
// =================================================================================

## endfor