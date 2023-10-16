#ifndef SUNGEARENGINE_ECSWORLD_H
#define SUNGEARENGINE_ECSWORLD_H

#include "IComponent.h"
#include "ISystem.h"
#include "Scene.h"
#include "SGCore/Utils/Utils.h"
#include <map>
#include <vector>

namespace Core::ECS
{
    struct MappedComponents
    {
        // first - hash of component
        // second - components with this hash
        std::map<size_t, std::set<std::shared_ptr<IComponent>>> m_components;
    };

    struct SystemCachedComponents
    {
        // first - the entity whose components were cached
        // second - MappedComponents
        std::map<std::shared_ptr<Entity>, MappedComponents> m_entitiesComponents;
    };

    // TODO: add allocator and free for components that will remove component from cached components if free
    // TODO: when the component is free, the allocator will make a signal to the systems that cache this type of components for the recache
    class ECSWorld
    {
    private:
        static inline std::list<std::shared_ptr<ISystem>> m_systems;

        // first - hash code of component class
        // second - EntityCachedComponents
        static inline std::map<size_t, SystemCachedComponents> m_cachedComponents;

    public:
        static void init() noexcept;

        template<typename ComponentT>
        requires(std::is_base_of_v<IComponent, ComponentT>)
        static void cacheComponent(const size_t& systemHashCode,
                                   const std::shared_ptr<Entity>& entity,
                                   const size_t& componentHashCode,
                                   const std::shared_ptr<ComponentT>& component)
        {
            m_cachedComponents[systemHashCode].m_entitiesComponents[entity].m_components[componentHashCode].insert(component);
        }

        template<typename SystemT, typename... ComponentsT>
        static void cacheComponents(const std::shared_ptr<Entity>& entity)
        {
            size_t systemHashCode = typeid(SystemT).hash_code();

            Utils::Utils::forTypes<ComponentsT...>([&entity, &systemHashCode](auto t)
            {
                using type = typename decltype(t)::type;

                size_t TComponentHashCode = typeid(type).hash_code();

                std::list<std::shared_ptr<type>> components = entity->getComponents<type>();

                for(const auto& component : components)
                {
                    ECSWorld::cacheComponent(systemHashCode, entity, TComponentHashCode, component);
                }
            });
        }

        __forceinline static SystemCachedComponents getSystemCachedComponents(const size_t& systemHashCode) noexcept
        {
            return m_cachedComponents[systemHashCode];
        }

        static void FPSNotRelativeFixedUpdate(const std::shared_ptr<Scene>& scene);
        static void FPSRelativeFixedUpdate(const std::shared_ptr<Scene>& scene);

        template<typename SystemT>
        requires(std::is_base_of_v<ISystem, SystemT>)
        static std::shared_ptr<SystemT> createSystem()
        {
            std::shared_ptr<SystemT> newSystem = std::make_shared<SystemT>();
            newSystem->addFlag(SystemsFlags::SGSF_PER_ENTITY);

            m_systems.push_back(newSystem);

            return newSystem;
        }
    };
}

#endif //SUNGEARENGINE_ECSWORLD_H
