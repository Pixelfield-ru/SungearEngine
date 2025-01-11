//
// Created by stuka on 25.01.2024.
//

#ifndef SUNGEARENGINE_ENTITYBASEINFO_H
#define SUNGEARENGINE_ENTITYBASEINFO_H

#include <SGCore/pch.h>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/UniqueName.h"
#include "SGCore/ECS/Component.h"
#include "EntityRef.h"

namespace SGCore
{
    struct Layer;

    // sizeof
    struct EntityBaseInfo : public UniqueNameWrapper, public ECS::Component<EntityBaseInfo, const EntityBaseInfo>
    {
        sg_serde_as_friend()

        explicit EntityBaseInfo(const ECS::entity_t& thisEntity) noexcept
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

        void setParent(const ECS::entity_t& parent, ECS::registry_t& inRegistry) noexcept;
        void detachFromParent(ECS::registry_t& inRegistry) noexcept;

        void addChild(const ECS::entity_t& child, ECS::registry_t& inRegistry) noexcept;
        void removeChild(const ECS::entity_t& child, ECS::registry_t& inRegistry) noexcept;
        [[nodiscard]] bool hasChild(const ECS::entity_t& child) const noexcept;

        [[nodiscard]] const std::vector<ECS::entity_t>& getChildren() const noexcept;

        [[nodiscard]] ECS::entity_t getParent() const noexcept;

        /**
         * @param allEntitiesInTree (OPTIONAL) The entities of the entire tree, starting from the root entity, ending with the entity for which this function was called.
         * @param inRegistry In which register the search will be performed.
         * @return The root parent, whose parent is the scene itself!
         */
        [[nodiscard]] ECS::entity_t getRootParent(ECS::registry_t& inRegistry) const noexcept;

        /**
         * @param inRegistry In which register the collecting will be performed.
         * @param outputEntities The entities of the entire tree, starting from this entity.
         */
        [[nodiscard]] void getAllChildren(ECS::registry_t& inRegistry, std::vector<ECS::entity_t>& outputEntities) const noexcept;

        /**
         * Searches for an entity by name starting with this entity.
         * @param inRegistry
         * @param name The name of the entity to be found
         * @return Found entity or \p entt::null .
         */
        [[nodiscard]] ECS::entity_t findEntity(ECS::registry_t& inRegistry, const std::string& name) const noexcept;

        [[nodiscard]] const glm::vec3& getUniqueColor() const noexcept;

        [[nodiscard]] const ECS::entity_t& getThisEntity() const noexcept;

        void resolveAllEntitiesRefs(const Ref<ECS::registry_t>& registry) noexcept;

        void generateUniqueColor() noexcept;

        EntityBaseInfo& operator=(const EntityBaseInfo&) = default;
        EntityBaseInfo& operator=(EntityBaseInfo&&) = default;

    private:
        EntityBaseInfo() = default;

        void setThisEntity(const ECS::entity_t& entity) noexcept;

        ECS::entity_t m_parent = entt::null;
        // used to resolve all references to this entity
        ECS::entity_t m_deserializedThisEntity = entt::null;
        // DO NOT WRITE AND READ THIS MEMBER DIRECTLY IF YOU ARE FRIEND OF THIS STRUCT
        ECS::entity_t m_thisEntity = entt::null;

        // IS EDITING ONLY IN StandardSerdeSpecs.h! CONTAINS ALL EntityRef`s OF THIS ENTITY THAT ARE NEED TO BE RESOLVED AFTER DESERIALIZATION
        std::vector<Ref<ECS::entity_t>> m_entitiesRefsToResolve;

        // for picking. not serializable
        glm::vec3 m_uniqueColor { };

        std::vector<ECS::entity_t> m_children;
    };
}

#endif //SUNGEARENGINE_ENTITYBASEINFO_H
