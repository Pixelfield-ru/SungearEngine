//
// Created by stuka on 25.01.2024.
//

#ifndef SUNGEARENGINE_ENTITYBASEINFO_H
#define SUNGEARENGINE_ENTITYBASEINFO_H

#include <SGCore/pch.h>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/UniqueName.h"
#include "SGCore/Utils/ECS/ComponentTraits.h"
#include "EntityRef.h"

namespace SGCore
{
    struct Layer;

    // sizeof
    struct EntityBaseInfo : public UniqueNameWrapper, public ComponentTraits<EntityBaseInfo>
    {
        sg_serde_as_friend()

        explicit EntityBaseInfo(const entity_t& thisEntity) noexcept
        {
            setThisEntity(thisEntity);
        }
        EntityBaseInfo(const EntityBaseInfo&) = default;
        EntityBaseInfo(EntityBaseInfo&&) = default;

        static inline glm::vec4 s_outlineColor0 = { 0.5, 0.5, 0.0, 1.0 };
        static inline glm::vec4 s_outlineColor1 = { 0.0, 0.5, 0.5, 1.0 };

        glm::vec4 m_outlineColor = s_outlineColor0;

        // sg_member()
        Weak<Layer> m_layer;

        void setParent(const entity_t& parent, registry_t& inRegistry) noexcept;
        void detachFromParent(registry_t& inRegistry) noexcept;

        void addChild(const entity_t& child, registry_t& inRegistry) noexcept;
        void removeChild(const entity_t& child, registry_t& inRegistry) noexcept;
        [[nodiscard]] bool hasChild(const entity_t& child) const noexcept;

        [[nodiscard]] const std::vector<entity_t>& getChildren() const noexcept;

        [[nodiscard]] entity_t getParent() const noexcept;

        /**
         * @param allEntitiesInTree (OPTIONAL) The entities of the entire tree, starting from the root entity, ending with the entity for which this function was called.
         * @param inRegistry In which register the search will be performed.
         * @return The root parent, whose parent is the scene itself!
         */
        [[nodiscard]] entity_t getRootParent(registry_t& inRegistry) const noexcept;

        /**
         * @param inRegistry In which register the collecting will be performed.
         * @param outputEntities The entities of the entire tree, starting from this entity.
         */
        [[nodiscard]] void getAllChildren(registry_t& inRegistry, std::vector<entity_t>& outputEntities) const noexcept;

        [[nodiscard]] const glm::vec3& getUniqueColor() const noexcept;

        [[nodiscard]] const entity_t& getThisEntity() const noexcept;

        void resolveAllEntitiesRefs(const Ref<registry_t>& registry) noexcept;

        void generateUniqueColor() noexcept;

        EntityBaseInfo& operator=(const EntityBaseInfo&) = default;
        EntityBaseInfo& operator=(EntityBaseInfo&&) = default;

    private:
        EntityBaseInfo() = default;

        void setThisEntity(const entity_t& entity) noexcept;

        entity_t m_parent = entt::null;
        // used to resolve all references to this entity
        entity_t m_deserializedThisEntity = entt::null;
        // DO NOT WRITE AND READ THIS MEMBER DIRECTLY IF YOU ARE FRIEND OF THIS STRUCT
        entity_t m_thisEntity = entt::null;

        // IS EDITING ONLY IN StandardSerdeSpecs.h! CONTAINS ALL EntityRef`s OF THIS ENTITY THAT ARE NEED TO BE RESOLVED AFTER DESERIALIZATION
        std::vector<Ref<entity_t>> m_entitiesRefsToResolve;

        // for picking. not serializable
        glm::vec3 m_uniqueColor { };

        std::vector<entity_t> m_children;
    };
}

#endif //SUNGEARENGINE_ENTITYBASEINFO_H
