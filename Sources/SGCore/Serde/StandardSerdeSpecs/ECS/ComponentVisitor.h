//
// Created by stuka on 16.08.2026.
//

#pragma once

#include "SGCore/Serde/Serde.h"
#include "SGCore/ECS/IComponentVisitor.h"

#define SG_SERDE_SPECIALIZE_ECS_COMPONENT(VisitorName)                                                      \
SG_SERDE_SPECIALIZE_ECS_COMPONENT_IMPL(VisitorName, SGCore::Serde::FormatType::JSON)                        \
SG_SERDE_SPECIALIZE_ECS_COMPONENT_IMPL(VisitorName, SGCore::Serde::FormatType::YAML)                        \
SG_SERDE_SPECIALIZE_ECS_COMPONENT_IMPL(VisitorName, SGCore::Serde::FormatType::BSON)                        \

#define SG_SERDE_SPECIALIZE_ECS_COMPONENT_IMPL(VisitorName, FormatType)                                                                                                             \
template <>                                                                                                                                                                         \
struct SGCORE_EXPORT VisitorName<FormatType> final : SGCore::Serde::ComponentVisitor<FormatType>                                                                                    \
{                                                                                                                                                                                   \
    SG_IMPLEMENT_TYPE_ID(VisitorName<FormatType>);                                                                                                                                  \
    void visit(SGCore::ECS::registry_t& registry, SGCore::ECS::entity_t entity, const std::function<void(SGCore::ECS::ComponentBase&, std::uint64_t)>& func) noexcept override;     \
};

#define SG_SERDE_DECLARE_ECS_COMPONENT(VisitorName)                                                                                                                                 \
template <SGCore::Serde::FormatType TFormatType>                                                                                                                                    \
struct VisitorName;

#define SG_SERDE_IMPLEMENT_ECS_COMPONENT(VisitorName, ComponentType)                                                    \
SG_SERDE_IMPLEMENT_ECS_COMPONENT_IMPL(VisitorName, ComponentType, SGCore::Serde::FormatType::JSON)                      \
// SG_SERDE_IMPLEMENT_ECS_COMPONENT_IMPL(VisitorName, ComponentType, SGCore::Serde::FormatType::YAML)                      \
// SG_SERDE_IMPLEMENT_ECS_COMPONENT_IMPL(VisitorName, ComponentType, SGCore::Serde::FormatType::BSON)                      \

#define SG_SERDE_IMPLEMENT_ECS_COMPONENT_IMPL(VisitorName, ComponentType, FormatType)                                                                                                           \
void VisitorName<FormatType>::visit(SGCore::ECS::registry_t& registry, SGCore::ECS::entity_t entity, const std::function<void(SGCore::ECS::ComponentBase&, std::uint64_t)>& func) noexcept      \
{                                                                                                                                                                                               \
    if(m_serOrDeser)                                                                                                                                                                            \
    {                                                                                                                                                                                           \
        auto* component = registry.tryGet<ComponentType>(entity);                                                                                                                               \
        if(!component) return;                                                                                                                                                                  \
        m_serializableValueView->container().addMember(SerdeSpec<ComponentType, FormatType>::type_name(), *component, entity, registry);                                                        \
    }                                                                                                                                                                                           \
    else                                                                                                                                                                                        \
    {                                                                                                                                                                                           \
        if(m_deserializableValueView->container().hasMember(SerdeSpec<ComponentType, FormatType>::type_name()))                                                                                 \
        {                                                                                                                                                                                       \
            auto component = m_deserializableValueView->container().template getMember<ComponentType::reg_t>(SerdeSpec<ComponentType, FormatType>::type_name());                                \
                                                                                                                                                                                                \
            if(component)                                                                                                                                                                       \
            {                                                                                                                                                                                   \
                registry.emplace<ComponentType>(entity, std::move(*component));                                                                                                                 \
            }                                                                                                                                                                                   \
        }                                                                                                                                                                                       \
    }                                                                                                                                                                                           \
}

namespace SGCore::Serde
{
    template<FormatType TFormatType>
    struct ComponentVisitor : public SGCore::ECS::IComponentVisitor
    {
        SerializableValueView<const ECS::entity_t, TFormatType>* m_serializableValueView {};
        DeserializableValueView<ECS::entity_t, TFormatType>* m_deserializableValueView {};

        /// True is serialization, false is deserialization
        bool m_serOrDeser = true;
    };
}
