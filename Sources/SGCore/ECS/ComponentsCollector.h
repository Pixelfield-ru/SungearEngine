//
// Created by stuka on 23.11.2023.
//

#ifndef SUNGEARENGINE_COMPONENTSCOLLECTOR_H
#define SUNGEARENGINE_COMPONENTSCOLLECTOR_H

#include <unordered_map>
#include <map>
#include <GLFW/glfw3.h>

#include "Entity.h"
#include "ComponentsCollection.h"
#include "Layer.h"
#include "SGCore/Utils/Utils.h"

namespace SGCore
{
    struct LayerCachedEntities
    {
        std::unordered_map<Ref<Entity>, ComponentsCollection> m_cachedEntities;
    };

    struct ComponentsCollector
    {
    private:
        using WillCachePredic = std::function<bool(const Ref<Entity>& entity)>;
        using WillComponentCachePredic = std::function<bool(const Ref<Entity>& entity, const Ref<IComponent>& component)>;

    public:
        std::map<Ref<Layer>, LayerCachedEntities, LayersComparator> m_cachedEntities;

        template<typename... ComponentsT>
        requires(std::is_base_of_v<IComponent, ComponentsT> && ...)
        void configureCachingFunction() noexcept
        {
            m_cachingFunction = [&](const Ref<Entity>& entity)
            {
                cacheEntityComponents<ComponentsT...>(entity);
            };
        }

        template<typename... ComponentsT>
        requires(std::is_base_of_v<IComponent, ComponentsT> && ...)
        void configureCachingFunction(const WillCachePredic& willCachePredicate)
        {
            m_cachingFunction = [&, willCachePredicate](const Ref<Entity>& entity)
            {
                cacheEntityComponents<ComponentsT...>(entity, willCachePredicate);
            };
        }

        template<typename... ComponentsT>
        requires(std::is_base_of_v<IComponent, ComponentsT> && ...)
        void configureCachingFunction(const WillComponentCachePredic& willCacheComponentPredicate)
        {
            m_cachingFunction = [&, willCacheComponentPredicate](const Ref<Entity>& entity)
            {
                cacheEntityComponents<ComponentsT...>(entity,
                                                      [](const Ref<Entity>& entity, const Ref<IComponent>& component) { return true; }, willCacheComponentPredicate);
            };
        }

        template<typename... ComponentsT>
        requires(std::is_base_of_v<IComponent, ComponentsT> && ...)
        void configureCachingFunction(const WillCachePredic& willCachePredicate,
                                      const WillComponentCachePredic& willCacheComponentPredicate)
        {
            m_cachingFunction = [&, willCachePredicate, willCacheComponentPredicate](const Ref<Entity>& entity)
            {
                cacheEntityComponents<ComponentsT...>(entity, willCachePredicate, willCacheComponentPredicate);
            };
        }

        void cacheEntity(const Ref<Entity>& entity) const noexcept
        {
            if(m_cachingFunction) m_cachingFunction(entity);
        }

    private:
        template<typename... ComponentsT>
        requires(std::is_base_of_v<IComponent, ComponentsT> && ...)
        void cacheEntityComponents(const Ref<Entity>& entity,
                                   const WillCachePredic& willCachePredicate,
                                   const WillComponentCachePredic& willCacheComponentPredicate)
        {
            double t0 = glfwGetTime();

            if(!willCachePredicate(entity)) return;

            auto entityLayer = entity->getLayer();
            if(!entityLayer) return;

            // is components of all ComponentsT... exists
            bool componentsSetExistsInEntity = true;
            Utils::Utils::forTypes<ComponentsT...>([&entity, &componentsSetExistsInEntity](auto t)
                                                   {
                                                       using type = typename decltype(t)::type;

                                                       if(!entity->getComponent<type>())
                                                       {
                                                           componentsSetExistsInEntity = false;
                                                           return;
                                                       }
                                                   });

            // if not exists that we wil not cache components of this entity
            if(!componentsSetExistsInEntity) return;

            auto& foundComponentsCollection = m_cachedEntities[entityLayer].m_cachedEntities[entity];

            Utils::Utils::forTypes<ComponentsT...>([&entity, &foundComponentsCollection, &willCacheComponentPredicate](auto t)
                                                   {
                                                       using type = typename decltype(t)::type;

                                                       // if component already exists in components collection then we wont cache
                                                       if (!foundComponentsCollection.getComponent<type>())
                                                       {
                                                           auto entityComponentsList = entity->getComponents<type>();
                                                           for (const auto& component : entityComponentsList)
                                                           {
                                                               if(willCacheComponentPredicate(entity, component))
                                                               {
                                                                   foundComponentsCollection.addComponent(component);
                                                               }
                                                           }
                                                       }
                                                   });

            double t1 = glfwGetTime();

            std::cout << "ms: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
        }

        template<typename... ComponentsT>
        requires(std::is_base_of_v<IComponent, ComponentsT> && ...)
        void cacheEntityComponents(const Ref<Entity>& entity,
                                   const WillCachePredic& willCachePredicate)
        {
            cacheEntityComponents<ComponentsT...>(entity, willCachePredicate,
                                                  [](const Ref<Entity>& entity, const Ref<IComponent>& component) { return true; });
        }

        template<typename... ComponentsT>
        requires(std::is_base_of_v<IComponent, ComponentsT> && ...)
        void cacheEntityComponents(const Ref<Entity>& entity,
                                   const WillComponentCachePredic& willCacheComponentPredicate)
        {
            cacheEntityComponents<ComponentsT...>(entity, [](const Ref<Entity>& entity) { return true; },
                                                  willCacheComponentPredicate);
        }

        template<typename... ComponentsT>
        requires(std::is_base_of_v<IComponent, ComponentsT> && ...)
        void cacheEntityComponents(const Ref<Entity>& entity)
        {
            cacheEntityComponents<ComponentsT...>(entity, [](const Ref<Entity>& entity) { return true; },
                                                  [](const Ref<Entity>& entity, const Ref<IComponent>& component) { return true; });
        }

        std::function<void(const Ref<Entity>& entity)> m_cachingFunction;
    };
}

#endif //SUNGEARENGINE_COMPONENTSCOLLECTOR_H
