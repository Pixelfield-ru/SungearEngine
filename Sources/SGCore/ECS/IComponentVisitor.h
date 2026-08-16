//
// Created by stuka on 12.08.2026.
//

#pragma once

#include <functional>

#include "SGCore/Main/CoreGlobals.h"
#include "sgcore_export.h"
#include "SGCore/Utils/StaticTypeID.h"

#define SG_DECLARE_ECS_COMPONENT_DEFAULT_VISITOR(visitor_name)                                                                                                                      \
struct SGCORE_EXPORT visitor_name final : SGCore::ECS::IComponentVisitor                                                                                                            \
{                                                                                                                                                                                   \
    SG_IMPLEMENT_TYPE_ID(visitor_name);                                                                                                                                             \
    void visit(SGCore::ECS::registry_t& registry, SGCore::ECS::entity_t entity, const std::function<void(SGCore::ECS::ComponentBase&, std::uint64_t)>& func) noexcept override;     \
};

#define SG_IMPLEMENT_ECS_COMPONENT_DEFAULT_VISITOR(visitor_name, component_type)                                                                                                        \
void visitor_name::visit(SGCore::ECS::registry_t& registry, SGCore::ECS::entity_t entity, const std::function<void(SGCore::ECS::ComponentBase&, std::uint64_t)>& func) noexcept         \
{                                                                                                                                                                                       \
    auto* component = registry.tryGet<component_type>(entity);                                                                                                                          \
    if(!component) return;                                                                                                                                                              \
                                                                                                                                                                                        \
    func(*component, component_type::getTypeIDStatic());                                                                                                                                \
}

namespace SGCore::ECS
{
    struct ComponentBase;

    struct SGCORE_EXPORT IComponentVisitor
    {
        SG_IMPLEMENT_TYPE_ID_BASE(SGCore::ECS::IComponentVisitor);

        virtual ~IComponentVisitor() = default;

        virtual void visit(registry_t& registry, entity_t entity, const std::function<void(ComponentBase&, std::uint64_t)>& func) = 0;
    };

    struct SGCORE_EXPORT VisitorsRegistry
    {
        void registerVisitor(const Ref<IComponentVisitor>& visitor) noexcept;

        void visit(registry_t& registry, entity_t entity, const std::function<void(ComponentBase&, std::uint64_t)>& func) const noexcept;
        const std::vector<Ref<IComponentVisitor>>& visitors() const noexcept;

        static VisitorsRegistry& instance() noexcept;

    private:
        std::vector<Ref<IComponentVisitor>> m_visitors;
    };
}
