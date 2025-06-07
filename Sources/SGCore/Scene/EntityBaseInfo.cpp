//
// Created by stuka on 09.10.2024.
//

#include "EntityBaseInfo.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/ECS/Registry.h"
#include "SGCore/Utils/Assert.h"

void SGCore::EntityBaseInfo::setParent(const SGCore::ECS::entity_t& parent,
                                       SGCore::ECS::registry_t& inRegistry) noexcept
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

    parentBaseInfo->m_children.push_back(m_thisEntity);
}

void SGCore::EntityBaseInfo::detachFromParent(SGCore::ECS::registry_t& inRegistry) noexcept
{
    setParent(entt::null, inRegistry);
}

void SGCore::EntityBaseInfo::addChild(const SGCore::ECS::entity_t& child,
                                      SGCore::ECS::registry_t& inRegistry) noexcept
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
}

void SGCore::EntityBaseInfo::removeChild(const SGCore::ECS::entity_t& child,
                                         SGCore::ECS::registry_t& inRegistry) noexcept
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
}

bool SGCore::EntityBaseInfo::hasChild(const SGCore::ECS::entity_t& child) const noexcept
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

void SGCore::EntityBaseInfo::destroy(ECS::registry_t& inRegistry) const noexcept
{
    for(const auto& child : m_children)
    {
#ifndef SUNGEAR_DEBUG
        inRegistry.get<EntityBaseInfo>(child).destroy(inRegistry);
#else
        auto* childEntityBaseInfo = inRegistry.tryGet<EntityBaseInfo>(child);
        SG_ASSERT(childEntityBaseInfo != nullptr, fmt::format("Can not destroy child entity '{}' of parent entity '{}': child entity does not have component EntityBaseInfo.",
            std::to_underlying(m_thisEntity),
            std::to_underlying(child)).c_str());

        childEntityBaseInfo->destroy(inRegistry);
#endif
    }

    SG_ASSERT(inRegistry.valid(m_thisEntity), fmt::format("Can not destroy entity '{}': invalid entity.", std::to_underlying(m_thisEntity)).c_str());
    inRegistry.destroy(m_thisEntity);
}

const glm::vec3& SGCore::EntityBaseInfo::getUniqueColor() const noexcept
{
    return m_uniqueColor;
}

const SGCore::ECS::entity_t& SGCore::EntityBaseInfo::getThisEntity() const noexcept
{
    return m_thisEntity;
}

void SGCore::EntityBaseInfo::resolveAllEntitiesRefs(const SGCore::Ref<SGCore::ECS::registry_t>& registry) noexcept
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

void SGCore::EntityBaseInfo::generateUniqueColor() noexcept
{
    const auto underlyingVal = std::to_underlying(m_thisEntity);

    m_uniqueColor.x = (float) (((underlyingVal + 1) & 0xFF0000) >> 16) / 255.0f;
    m_uniqueColor.y = (float) (((underlyingVal + 1) & 0x00FF00) >> 8) / 255.0f;
    m_uniqueColor.z = (float) ((underlyingVal + 1) & 0x0000FF) / 255.0f;

    std::cout << fmt::format("UNIQUE COLOR x = {}, y = {}, z = {}",
                             m_uniqueColor.x, m_uniqueColor.y, m_uniqueColor.z) << std::endl;
}

void SGCore::EntityBaseInfo::setThisEntity(const SGCore::ECS::entity_t& entity) noexcept
{
    m_thisEntity = entity;
    generateUniqueColor();
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

void SGCore::EntityBaseInfo::getAllChildren(SGCore::ECS::registry_t& inRegistry,
                                            std::vector<ECS::entity_t>& outputEntities) const noexcept
{
    outputEntities.push_back(m_thisEntity);

    for(const auto& e : m_children)
    {
        auto* childBaseInfo = inRegistry.tryGet<EntityBaseInfo>(e);
        outputEntities.push_back(e);

        if(childBaseInfo)
        {
            childBaseInfo->getAllChildren(inRegistry, outputEntities);
        }
    }
}

SGCore::ECS::entity_t SGCore::EntityBaseInfo::findEntity(SGCore::ECS::registry_t& inRegistry,
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
        else
        {
            return childBaseInfo.findEntity(inRegistry, name);
        }
    }

    return entt::null;
}
