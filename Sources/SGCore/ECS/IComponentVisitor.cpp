//
// Created by stuka on 12.08.2026.
//

#include "IComponentVisitor.h"

void SGCore::ECS::VisitorsRegistry::registerVisitor(const Ref<IComponentVisitor>& visitor) noexcept
{
    m_visitors.push_back(visitor);
}

void SGCore::ECS::VisitorsRegistry::visit(registry_t& registry, entity_t entity,
                                          const std::function<void(ComponentBase&, std::uint64_t)>& func) const noexcept
{
    for(const auto& visitor : m_visitors)
    {
        visitor->visit(registry, entity, func);
    }
}

const std::vector<SGCore::Ref<SGCore::ECS::IComponentVisitor>>& SGCore::ECS::VisitorsRegistry::visitors() const noexcept
{
    return m_visitors;
}

SGCore::ECS::VisitorsRegistry& SGCore::ECS::VisitorsRegistry::instance() noexcept
{
    static VisitorsRegistry registry;
    return registry;
}
