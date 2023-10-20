#ifndef SUNGEARENGINE_ECSWORLD_H
#define SUNGEARENGINE_ECSWORLD_H

#include "IComponent.h"
#include "ISystem.h"
#include "Scene.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Patterns/UUID.h"
#include <unordered_map>
#include <vector>
#include <robin_map/include/tsl/robin_map.h>
#include "ComponentsCollection.h"

namespace Core::ECS
{
    struct SystemCachedEntities
    {
        tsl::robin_map<std::shared_ptr<Entity>, std::shared_ptr<ComponentsCollection>> m_cachedEntities;
    };

    // TODO: add allocator and free for components that will remove component from cached components if free
    // TODO: when the component is free, the allocator will make a signal to the systems that cache this type of components for the recache
    class ECSWorld
    {
    public:
        static void init() noexcept;

        static void FPSNotRelativeFixedUpdate(const std::shared_ptr<Scene>& scene);

        static void FPSRelativeFixedUpdate(const std::shared_ptr<Scene>& scene);

        template<typename SystemT, typename... ComponentsT>
        requires(std::is_base_of_v<ISystem, SystemT> && (std::is_base_of_v<IComponent, ComponentsT> && ...))
        static void cacheComponents(const std::shared_ptr<Entity>& entity)
        {
            const size_t systemTID = typeid(SystemT).hash_code();

            auto& systemCachedEntities = m_cachedComponentsCollections[systemTID];
            systemCachedEntities = systemCachedEntities == nullptr ?
                                   std::make_shared<SystemCachedEntities>() : systemCachedEntities;

            // is components of all ComponentsT... exists
            bool componentsSetExists = true;
            Utils::Utils::forTypes<ComponentsT...>([&entity, &componentsSetExists](auto t)
            {
                using type = typename decltype(t)::type;

                if(entity->getComponents<type>().empty())
                {
                    componentsSetExists = false;
                    return;
                }
            });

            // if not exists that we wil not cache components of this entity
            if(!componentsSetExists) return;

            Utils::Utils::forTypes<ComponentsT...>([&entity, &systemCachedEntities](auto t)
            {
                using type = typename decltype(t)::type;

                auto& foundComponentsCollection = systemCachedEntities->m_cachedEntities[entity];
                foundComponentsCollection = foundComponentsCollection == nullptr ?
                        std::make_shared<ComponentsCollection>() : foundComponentsCollection;

                auto entityComponentsList = entity->getComponents<type>();
                for(const auto& component : entityComponentsList)
                {
                    foundComponentsCollection->addComponent(component);
                }
            });
        }

        template<typename SystemT>
        requires(std::is_base_of_v<ISystem, SystemT>)
        static std::shared_ptr<SystemCachedEntities> getSystemCachedEntities()
        {
            return m_cachedComponentsCollections[typeid(SystemT).hash_code()];
        }

        template<typename SystemT>
        requires(std::is_base_of_v<ISystem, SystemT>)
        static std::shared_ptr<SystemT> createSystem()
        {
            std::shared_ptr<SystemT> newSystem = std::make_shared<SystemT>();
            newSystem->addFlag(SystemsFlags::SGSF_PER_ENTITY);

            m_systems.push_back(newSystem);

            return newSystem;
        }

        template<typename ComponentT>
        requires(std::is_base_of_v<IComponent, ComponentT>)
        static std::shared_ptr<ComponentT> createComponent()
        {
            auto component = std::make_shared<ComponentT>();

        }

    private:
        static inline tsl::robin_map<size_t, std::shared_ptr<SystemCachedEntities>> m_cachedComponentsCollections;
        //static inline tsl::robin_map<size_t,
        static inline std::list<std::shared_ptr<ISystem>> m_systems;
    };
}

#endif //SUNGEARENGINE_ECSWORLD_H
