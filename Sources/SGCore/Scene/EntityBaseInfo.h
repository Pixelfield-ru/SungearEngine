//
// Created by stuka on 25.01.2024.
//

#ifndef SUNGEARENGINE_ENTITYBASEINFO_H
#define SUNGEARENGINE_ENTITYBASEINFO_H

#include <SGCore/pch.h>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/UniqueName.h"

namespace SGCore
{
    struct Layer;

    // sizeof
    struct EntityBaseInfo : public UniqueNameWrapper
    {
        sg_serde_as_friend()

        explicit EntityBaseInfo(const entity_t& thisEntity) noexcept : m_thisEntity(thisEntity) { }
        EntityBaseInfo(const EntityBaseInfo&) = default;
        EntityBaseInfo(EntityBaseInfo&&) = default;

        // sg_member()
        Weak<Layer> m_layer;

        void setParent(const entity_t& parent, registry_t& inRegistry) noexcept;
        void detachFromParent(registry_t& inRegistry) noexcept;

        void addChild(const entity_t& child, registry_t& inRegistry) noexcept;
        void removeChild(const entity_t& child, registry_t& inRegistry) noexcept;
        [[nodiscard]] bool hasChild(const entity_t& child) const noexcept;

        [[nodiscard]] const std::vector<entity_t>& getChildren() const noexcept;

        [[nodiscard]] entity_t getParent() const noexcept;

        EntityBaseInfo& operator=(const EntityBaseInfo&) = default;
        EntityBaseInfo& operator=(EntityBaseInfo&&) = default;

    private:
        EntityBaseInfo() = default;

        entity_t m_parent = entt::null;
        entity_t m_thisEntity = entt::null;

        std::vector<entity_t> m_children;
    };
}

#endif //SUNGEARENGINE_ENTITYBASEINFO_H
