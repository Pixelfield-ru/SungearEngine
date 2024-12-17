// INFO FOR COMPONENTS SerdeSpec: YOU CAN USE ONLY THIS FUNCTIONS TO SERIALIZE AND DESERIALIZE YOUR COMPONENT:
// static void serialize(SGCore::Serde::SerializableValueView<T, TFormatType>& valueView) noexcept;
// static void serialize(SGCore::Serde::SerializableValueView<T, TFormatType>& valueView, const SGCore::entity_t& entity, SGCore::registry_t& registry) noexcept;
// static void deserialize(SGCore::Serde::DeserializableValueView<T, TFormatType>& valueView) noexcept;
// static void deserialize(SGCore::Serde::DeserializableValueView<T, TFormatType>& valueView, const SGCore::entity_t& entity, SGCore::registry_t& registry) noexcept;

#pragma once

#include <SGCore/Serde/Serde.h>
#include <SGCore/Serde/StandardSerdeSpecs.h>

## for struct in structs
## if !struct.hasMember(name: "annotations") || !struct.annotations.hasMember(name: "doNotGenerateSerdeSpec")
## if struct.hasMember(name: "filePath")
#include "{{ struct.filePath }}"
## endif
## endif
## endfor

## for struct in structs

## if !struct.hasMember(name: "annotations") || !struct.annotations.hasMember(name: "doNotGenerateSerdeSpec")
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

    static void serialize(SGCore::Serde::SerializableValueView<{{ struct.fullNameWithTemplate }}, TFormatType>& valueView
## if struct.hasMember(name: "type") && struct.type.equals(value: "component")
        , const SGCore::entity& serializableEntity,
        SGCore::registry_t& sceneRegistry
## endif
        ) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<{{ struct.fullNameWithTemplate }}, TFormatType>& valueView
## if struct.hasMember(name: "type") && struct.type.equals(value: "component")
        , const SGCore::entity& deserializableEntity,
        SGCore::registry_t& sceneRegistry
## endif
        ) noexcept;
};
// =================================================================================

## endif
## endif

## endfor

// implementations

## for struct in structs

## if struct.hasMember(name: "fullName")
## if !struct.hasMember(name: "annotations") || !struct.annotations.hasMember(name: "doNotGenerateSerdeSpec")

// SERDE IMPL FOR struct '{{ struct.fullName }}'
// =================================================================================
template<
## if struct.hasMember(name: "templateDecl")
        {{ struct.templateDecl }},
## endif
        SGCore::Serde::FormatType TFormatType
        >
void SGCore::Serde::SerdeSpec<{{ struct.fullNameWithTemplate }}, TFormatType>::serialize(SGCore::Serde::SerializableValueView<{{ struct.fullNameWithTemplate }}, TFormatType>& valueView
## if struct.hasMember(name: "type") && struct.type.equals(value: "component")
        , const SGCore::entity& serializableEntity,
        SGCore::registry_t& sceneRegistry
## endif
        ) noexcept
{
    ## if struct.hasMember(name: "members")
    ## for member in struct.members

    ## if !member.hasMember(name: "annotations") || !member.annotations.hasMember(name: "doNotSerialize")

    ## if member.hasMember(name: "getter")
    valueView.getValueContainer().addMember("{{ member.name }}", valueView.m_data->{{ member.getter }}()
## if struct.hasMember(name: "type") && struct.type.equals(value: "component")
    , serializableEntity, sceneRegistry
## endif
    );
    ## else
    valueView.getValueContainer().addMember("{{ member.name }}", valueView.m_data->{{ member.name }}
## if struct.hasMember(name: "type") && struct.type.equals(value: "component")
    , serializableEntity, sceneRegistry
## endif
    );
    ## endif

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
void SGCore::Serde::SerdeSpec<{{ struct.fullNameWithTemplate }}, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<{{ struct.fullNameWithTemplate }}, TFormatType>& valueView
## if struct.hasMember(name: "type") && struct.type.equals(value: "component")
        , const SGCore::entity& deserializableEntity,
        SGCore::registry_t& sceneRegistry
## endif
        ) noexcept
{
    ## if struct.hasMember(name: "members")
    ## for member in struct.members
    ## if !member.hasMember(name: "annotations") || !member.annotations.hasMember(name: "doNotSerialize")

    ## if member.hasMember(name: "getter")
    const auto {{ member.name }} = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->{{ member.getter }}())>>>("{{ member.name }}",
            ## if struct.hasMember(name: "type") && struct.type.equals(value: "component")
            , deserializableEntity, sceneRegistry
            ## endif
            );
    ## else
    const auto {{ member.name }} = valueView.getValueContainer().template getMember<decltype(valueView.m_data->{{ member.name }})>("{{ member.name }}"
            ## if struct.hasMember(name: "type") && struct.type.equals(value: "component")
            , deserializableEntity, sceneRegistry
            ## endif
            );
    ## endif

    if({{ member.name }})
    {
        ## if member.hasMember(name: "setter")
        valueView.m_data->{{ member.setter }}(*{{ member.name }});
        ## else
        valueView.m_data->{{ member.name }} = *{{ member.name }};
        ## endif
    }

    ## endif
    ## endfor
    ## endif
}
// =================================================================================

## endif
## endif

## endfor