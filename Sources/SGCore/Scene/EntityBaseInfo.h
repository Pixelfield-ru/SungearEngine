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

        explicit EntityBaseInfo(const entity_t& thisEntity) noexcept : m_thisEntity(thisEntity)
        {
            const auto underlyingVal = std::to_underlying(thisEntity);

            m_uniqueColor.x = (float) ((underlyingVal & 0xFF0000) >> 16) / 255.0f;
            m_uniqueColor.y = (float) ((underlyingVal & 0x00FF00) >> 8) / 255.0f;
            m_uniqueColor.z = (float) (underlyingVal & 0x0000FF) / 255.0f;

            std::cout << std::format("UNIQUE COLOR x = {}, y = {}, z = {}",
                                     m_uniqueColor.x, m_uniqueColor.y, m_uniqueColor.z) << std::endl;
        }
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

        [[nodiscard]] const glm::vec3& getUniqueColor() const noexcept;

        EntityBaseInfo& operator=(const EntityBaseInfo&) = default;
        EntityBaseInfo& operator=(EntityBaseInfo&&) = default;

    private:
        EntityBaseInfo() = default;

        entity_t m_parent = entt::null;
        // used to resolve all references to this entity
        entity_t m_deserializedThisEntity = entt::null;
        entity_t m_thisEntity = entt::null;

        // for picking. not serializable
        glm::vec3 m_uniqueColor { };

        std::vector<entity_t> m_children;
    };
}

#endif //SUNGEARENGINE_ENTITYBASEINFO_H
