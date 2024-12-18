//
// Created by stuka on 09.10.2024.
//

#include "EntityBaseInfo.h"
#include "SGCore/Logger/Logger.h"

void SGCore::EntityBaseInfo::setParent(const SGCore::entity_t& parent,
                                       SGCore::registry_t& inRegistry) noexcept
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

        auto* parentBaseInfo = inRegistry.try_get<EntityBaseInfo>(m_parent);
        if(!parentBaseInfo)
        {
            LOG_E(SGCORE_TAG, "Can not detach entity '{}' from entity '{}'. Entity '{}' (parent) does not have EntityBaseInfo component.",
                  std::to_underlying(m_thisEntity),
                  std::to_underlying(m_parent),
                  std::to_underlying(m_parent));
            return;
        }

        m_parent = entt::null;

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

    auto* parentBaseInfo = inRegistry.try_get<EntityBaseInfo>(parent);
    if(!parentBaseInfo)
    {
        LOG_E(SGCORE_TAG, "Can not attach entity '{}' to entity '{}'. Entity '{}' (parent) does not have EntityBaseInfo component.",
              std::to_underlying(m_thisEntity),
              std::to_underlying(parent),
              std::to_underlying(parent));
        return;
    }

    m_parent = parent;

    parentBaseInfo->m_children.push_back(m_thisEntity);
}

void SGCore::EntityBaseInfo::detachFromParent(SGCore::registry_t& inRegistry) noexcept
{
    setParent(entt::null, inRegistry);
}

void SGCore::EntityBaseInfo::addChild(const SGCore::entity_t& child,
                                      SGCore::registry_t& inRegistry) noexcept
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

    auto* childBaseInfo = inRegistry.try_get<EntityBaseInfo>(child);
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
}

void SGCore::EntityBaseInfo::removeChild(const SGCore::entity_t& child,
                                         SGCore::registry_t& inRegistry) noexcept
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

    auto* childBaseInfo = inRegistry.try_get<EntityBaseInfo>(child);
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
}

bool SGCore::EntityBaseInfo::hasChild(const SGCore::entity_t& child) const noexcept
{
    return std::find(m_children.begin(), m_children.end(), child) != m_children.end();
}

const std::vector<SGCore::entity_t>& SGCore::EntityBaseInfo::getChildren() const noexcept
{
    return m_children;
}

SGCore::entity_t SGCore::EntityBaseInfo::getParent() const noexcept
{
    return m_parent;
}

const glm::vec3& SGCore::EntityBaseInfo::getUniqueColor() const noexcept
{
    return m_uniqueColor;
}

const SGCore::entity_t& SGCore::EntityBaseInfo::getThisEntity() const noexcept
{
    return m_thisEntity;
}

const void
SGCore::EntityBaseInfo::resolveAllEntitiesRefs(const SGCore::Ref<SGCore::registry_t>& registry) noexcept
{
    auto entityBaseInfoView = registry->template view<EntityBaseInfo>();

    auto it = m_entitiesRefsToResolve.begin();
    while(it != m_entitiesRefsToResolve.end())
    {
        bool isRefResolved = false;
        entityBaseInfoView.each([&it, &isRefResolved, this](const EntityBaseInfo& otherEntityBaseInfo) {
            // if deserialized value of EntityRef is equals to deserialized entity of other EntityBaseInfo
            // then we are resolving current EntityRef to entity of otherEntityBaseInfo
            if(**it == otherEntityBaseInfo.m_deserializedThisEntity)
            {
                **it = otherEntityBaseInfo.m_thisEntity;
                it = m_entitiesRefsToResolve.erase(it);
            }
            else
            {
                isRefResolved = true;
            }
        });

        if(!isRefResolved)
        {
            ++it;
        }
    }
}
