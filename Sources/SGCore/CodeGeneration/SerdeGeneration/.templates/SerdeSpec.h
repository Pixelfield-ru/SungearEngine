#pragma once

#include <SGCore/Serde/Serde.h>
#include <SGCore/Serde/StandardSerdeSpecs.h>

## for struct in structs
## if struct.hasMember(name: "filePath")
#include "{{ struct.filePath }}"
## endif
## endfor

## for struct in structs

## if struct.hasMember(name: "fullName")

// SERDE FORWARD DECL FOR struct '{{ struct.fullName }}'
// =================================================================================
template<
## if struct.hasMember(name: "templateDecl")
        {{ struct.templateDecl }},
## endif
        SGCore::Serde::FormatType TFormatType
        >
struct SGCore::Serde::SerdeSpec<{{ struct.fullNameWithTemplate }}, TFormatType> :
        SGCore::Serde::BaseTypes<
## if struct.hasMember(name: "baseTypes")
            {{ struct.baseTypes.place(separator: ", ") }}
## endif
        >,
        SGCore::Serde::DerivedTypes<
## if struct.hasMember(name: "derivedTypes")
            {{ struct.derivedTypes.place(separator: ", ") }}
## endif
        >
{
    static inline const std::string type_name = "{{ struct.fullName }}";
## if struct.hasMember(name: "isPointerType")
    static inline constexpr bool is_pointer_type = {{ struct.isPointerType }};
## else
    static inline constexpr bool is_pointer_type = false;
## endif

    static void serialize(SGCore::Serde::SerializableValueView<{{ struct.fullNameWithTemplate }}, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<{{ struct.fullNameWithTemplate }}, TFormatType>& valueView) noexcept;
};
// =================================================================================

## endif

## endfor

// implementations

## for struct in structs

## if struct.hasMember(name: "fullName")

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
    ## if struct.hasMember(name: "members")
    ## for member in struct.members

    ## if member.hasMember(name: "getter")
    valueView.getValueContainer().addMember("{{ member.name }}", valueView.m_data->{{ member.getter }}());
    ## else
    valueView.getValueContainer().addMember("{{ member.name }}", valueView.m_data->{{ member.name }});
    ## endif

    ## endfor
    ## endif
}

template<
## if struct.hasMember(name: "templateDecl")
        {{ struct.templateDecl }},
## endif
        SGCore::Serde::FormatType TFormatType
        >
void SGCore::Serde::SerdeSpec<{{ struct.fullNameWithTemplate }}, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<{{ struct.fullNameWithTemplate }}, TFormatType>& valueView) noexcept
{
    ## if struct.hasMember(name: "members")
    ## for member in struct.members

    ## if member.hasMember(name: "getter")
    const auto {{ member.name }} = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->{{ member.getter }}())>>>("{{ member.name }}");
    ## else
    const auto {{ member.name }} = valueView.getValueContainer().template getMember<decltype(valueView.m_data->{{ member.name }})>("{{ member.name }}");
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
    ## endif
}
// =================================================================================

## endif

## endfor