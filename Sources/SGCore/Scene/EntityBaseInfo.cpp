//
// Created by stuka on 09.10.2024.
//

#include "EntityBaseInfo.h"

#include "RootEntityTag.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/ECS/Registry.h"
#include "SGCore/Utils/Assert.h"
#include "SGCore/Utils/Defer.h"

SGCore::EntityBaseInfo::EntityBaseInfo(ECS::entity_t thisEntity, ECS::registry_t& inRegistry) noexcept
{
    m_thisEntity = thisEntity;
    inRegistry.emplace<RootEntityTag>(thisEntity);
}

void SGCore::EntityBaseInfo::setActiveRecursive(bool active, ECS::registry_t& inRegistry) noexcept
{
    m_isEntityActive = active;

    for(auto&& child : m_children)
    {
        auto& childEntityBaseInfo = inRegistry.get<EntityBaseInfo>(child);

        childEntityBaseInfo.setActiveRecursive(active, inRegistry);
    }
}

void SGCore::EntityBaseInfo::setParent(const ECS::entity_t& parent,
                                       ECS::registry_t& inRegistry) noexcept
{
    if(!inRegistry.valid(m_thisEntity))
    {
        LOG_E(SGCORE_TAG, "Can not set parent for entity '{}'. Entity '{}' (current) is not valid.",
              std::to_underlying(m_thisEntity),
              std::to_underlying(m_thisEntity));

        return;
    }

    // detaching this entity from parent =============================
    if(parent == entt::null)
    {
        if(m_parent == entt::null)
        {
            return;
        }

        if(!inRegistry.valid(m_parent))
        {
            LOG_E(SGCORE_TAG, "Can not detach entity '{}' from entity '{}'. Entity '{}' (parent) is not valid.",
                  std::to_underlying(m_thisEntity),
                  std::to_underlying(m_parent),
                  std::to_underlying(m_parent));
            return;
        }

        auto* parentBaseInfo = inRegistry.tryGet<EntityBaseInfo>(m_parent);
        if(!parentBaseInfo)
        {
            LOG_E(SGCORE_TAG, "Can not detach entity '{}' from entity '{}'. Entity '{}' (parent) does not have EntityBaseInfo component.",
                  std::to_underlying(m_thisEntity),
                  std::to_underlying(m_parent),
                  std::to_underlying(m_parent));
            return;
        }

        m_parent = entt::null;

        if(inRegistry.allOf<RootEntityTag>(m_thisEntity)) return;

        inRegistry.emplace<RootEntityTag>(m_thisEntity);

        parentBaseInfo->removeChild(m_thisEntity, inRegistry);

        return;
    }

    // else attaching this entity to new parent =======================

    // firstly detaching this entity from old parent
    setParent(entt::null, inRegistry);

    if(!inRegistry.valid(parent))
    {
        LOG_E(SGCORE_TAG, "Can not attach entity '{}' to entity '{}'. Entity '{}' (parent) is not valid.",
              std::to_underlying(m_thisEntity),
              std::to_underlying(parent),
              std::to_underlying(parent));
        return;
    }

    auto* parentBaseInfo = inRegistry.tryGet<EntityBaseInfo>(parent);
    if(!parentBaseInfo)
    {
        LOG_E(SGCORE_TAG, "Can not attach entity '{}' to entity '{}'. Entity '{}' (parent) does not have EntityBaseInfo component.",
              std::to_underlying(m_thisEntity),
              std::to_underlying(parent),
              std::to_underlying(parent));
        return;
    }

    m_parent = parent;

    inRegistry.remove<RootEntityTag>(m_thisEntity);

    parentBaseInfo->m_children.push_back(m_thisEntity);
}

void SGCore::EntityBaseInfo::detachFromParent(ECS::registry_t& inRegistry) noexcept
{
    setParent(entt::null, inRegistry);
}

void SGCore::EntityBaseInfo::addChild(const ECS::entity_t& child,
                                      ECS::registry_t& inRegistry) noexcept
{
    if(hasChild(child))
    {
        return;
    }

    if(!inRegistry.valid(m_thisEntity))
    {
        LOG_E(SGCORE_TAG, "Can not add child entity '{}' to entity '{}'. Entity '{}' (parent) is not valid.",
              std::to_underlying(child),
              std::to_underlying(m_thisEntity),
              std::to_underlying(m_thisEntity));
        return;
    }

    if(!inRegistry.valid(child))
    {
        LOG_E(SGCORE_TAG, "Can not add child entity '{}' to entity '{}'. Entity '{}' (child) is not valid.",
              std::to_underlying(child),
              std::to_underlying(m_thisEntity),
              std::to_underlying(child));
        return;
    }

    auto* childBaseInfo = inRegistry.tryGet<EntityBaseInfo>(child);
    if(!childBaseInfo)
    {
        LOG_E(SGCORE_TAG, "Can not add child entity '{}' to entity '{}'. Entity '{}' (child) does not have EntityBaseInfo component.",
              std::to_underlying(child),
              std::to_underlying(m_thisEntity),
              std::to_underlying(child));
        return;
    }

    m_children.push_back(child);
    childBaseInfo->m_parent = m_thisEntity;
    inRegistry.remove<RootEntityTag>(child);
}

void SGCore::EntityBaseInfo::removeChild(const ECS::entity_t& child,
                                         ECS::registry_t& inRegistry) noexcept
{
    if(!hasChild(child))
    {
        return;
    }

    if(!inRegistry.valid(m_thisEntity))
    {
        LOG_E(SGCORE_TAG, "Can not remove child entity '{}' from entity '{}'. Entity '{}' (parent) is not valid.",
              std::to_underlying(child),
              std::to_underlying(m_thisEntity),
              std::to_underlying(m_thisEntity));
        return;
    }

    if(!inRegistry.valid(child))
    {
        LOG_E(SGCORE_TAG, "Can not remove child entity '{}' from entity '{}'. Entity '{}' (child) is not valid.",
              std::to_underlying(child),
              std::to_underlying(m_thisEntity),
              std::to_underlying(child));
        return;
    }

    auto* childBaseInfo = inRegistry.tryGet<EntityBaseInfo>(child);
    if(!childBaseInfo)
    {
        LOG_E(SGCORE_TAG, "Can not remove child entity '{}' from entity '{}'. Entity '{}' (child) does not have EntityBaseInfo component.",
              std::to_underlying(child),
              std::to_underlying(m_thisEntity),
              std::to_underlying(child));
        return;
    }

    std::erase(m_children, child);
    childBaseInfo->m_parent = entt::null;
    inRegistry.emplace<RootEntityTag>(child);
}

bool SGCore::EntityBaseInfo::hasChild(const ECS::entity_t& child) const noexcept
{
    return std::find(m_children.begin(), m_children.end(), child) != m_children.end();
}

const std::vector<SGCore::ECS::entity_t>& SGCore::EntityBaseInfo::getChildren() const noexcept
{
    return m_children;
}

SGCore::ECS::entity_t SGCore::EntityBaseInfo::getParent() const noexcept
{
    return m_parent;
}

void SGCore::EntityBaseInfo::destroy(ECS::registry_t& inRegistry) noexcept
{
    if(inRegistry.valid(m_parent))
    {
        // removing this entity from parent
        auto& parentBaseInfo = inRegistry.get<EntityBaseInfo>(m_parent);
        std::erase(parentBaseInfo.m_children, m_thisEntity);
    }

    destroyImpl(inRegistry);
}

void SGCore::EntityBaseInfo::destroyImpl(ECS::registry_t& inRegistry) noexcept
{
    for(const auto& child : m_children)
    {
        inRegistry.get<EntityBaseInfo>(child).destroyImpl(inRegistry);
    }

    SG_ASSERT(inRegistry.valid(m_thisEntity), fmt::format("Can not destroy entity '{}': invalid entity.", std::to_underlying(m_thisEntity)).c_str());
    inRegistry.destroy(m_thisEntity);
}

SGCore::ECS::entity_t SGCore::EntityBaseInfo::getRootParent(ECS::registry_t& inRegistry) const noexcept
{
    auto* parentBaseInfo = inRegistry.tryGet<EntityBaseInfo>(m_parent);

    if(m_parent == entt::null || !parentBaseInfo)
    {
        return m_thisEntity;
    }

    return parentBaseInfo->getRootParent(inRegistry);
}

void SGCore::EntityBaseInfo::getAllChildren(ECS::registry_t& inRegistry,
                                            std::vector<ECS::entity_t>& outputEntities) const noexcept
{
    for(const auto& e : m_children)
    {
        auto& childBaseInfo = inRegistry.get<EntityBaseInfo>(e);
        outputEntities.push_back(e);
        childBaseInfo.getAllChildren(inRegistry, outputEntities);
    }
}

SGCore::ECS::entity_t SGCore::EntityBaseInfo::findEntity(ECS::registry_t& inRegistry,
                                                         const std::string& name) const noexcept
{
    if(getName() == name) return m_thisEntity;

    for(const auto& child : m_children)
    {
        EntityBaseInfo::reg_t& childBaseInfo = inRegistry.get<EntityBaseInfo>(child);

        if(childBaseInfo.getName() == name)
        {
            return childBaseInfo.m_thisEntity;
        }

        const auto foundEntity = childBaseInfo.findEntity(inRegistry, name);
        if(foundEntity != entt::null) return foundEntity;
    }

    return entt::null;
}

glm::vec3 SGCore::EntityBaseInfo::getUniqueColor() const noexcept
{
    return { m_uniqueColor.color().x, m_uniqueColor.color().y, m_uniqueColor.color().z };
}

void SGCore::EntityBaseInfo::resolveAllEntitiesRefs(const Ref<ECS::registry_t>& registry) noexcept
{
    auto entityBaseInfoView = registry->template view<EntityBaseInfo::reg_t>();

    auto it = m_entitiesRefsToResolve.begin();
    while(it != m_entitiesRefsToResolve.end())
    {
        bool isRefResolved = false;
        entityBaseInfoView.each([&it, &isRefResolved, this](const EntityBaseInfo::reg_t& otherEntityBaseInfo) {
            // if deserialized value of EntityRef is equals to deserialized entity of other EntityBaseInfo
            // then we are resolving current EntityRef to entity of otherEntityBaseInfo
            if(!isRefResolved && **it == otherEntityBaseInfo.m_deserializedThisEntity)
            {
                std::cout << "resolved entity ref. deserialized: " << std::to_underlying(otherEntityBaseInfo.m_deserializedThisEntity) <<
                        ", new: " << std::to_underlying(otherEntityBaseInfo.getThisEntity()) << std::endl;

                **it = otherEntityBaseInfo.getThisEntity();
                it = m_entitiesRefsToResolve.erase(it);
                isRefResolved = true;
            }
        });

        if(!isRefResolved)
        {
            ++it;
        }
    }
}

/*void SGCore::EntityBaseInfo::setThisEntity(ECS::entity_t entity) noexcept
{
    m_thisEntity = entity;
}*/
