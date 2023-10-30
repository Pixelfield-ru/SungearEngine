//
// Created by stuka on 02.05.2023.
//

#ifndef NATIVECORE_ISYSTEM_H
#define NATIVECORE_ISYSTEM_H

#include <functional>
#include <map>
#include <source_location>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "SGCore/Patterns/Marker.h"
#include "Transformations/TransformComponent.h"
#include "SGCore/Patterns/Singleton.h"

#include "ComponentsCollection.h"

namespace Core::ECS
{
    struct SystemsFlags
    {
        // system`s functions will call not per entity
        static inline const std::uint16_t SGSF_NOT_PER_ENTITY = 1;
        static inline const std::uint16_t SGSF_PER_ENTITY = 1 << 1;
    };

    class ISystem : public Patterns::Marker<ISystem>
    {
        friend class ECSWorld;
    public:
        bool m_active = true;

        virtual void fixedUpdate(const std::shared_ptr<Scene>& scene) { }
        virtual void update(const std::shared_ptr<Scene>& scene) { }

        virtual void fixedUpdate(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity) { }
        virtual void update(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity) { }

        virtual void cacheEntity(const std::shared_ptr<Entity>& entity) { }

        template<typename... ComponentsT>
        requires(std::is_base_of_v<IComponent, ComponentsT> && ...)
        void cacheEntityComponents(const std::shared_ptr<Entity>& entity)
        {
            double t0 = glfwGetTime();

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

            auto& foundComponentsCollection = m_cachedEntities[entityLayer][entity];
            foundComponentsCollection = !foundComponentsCollection ?
                                        std::make_shared<ComponentsCollection>() : foundComponentsCollection;

            Utils::Utils::forTypes<ComponentsT...>([&entity, &foundComponentsCollection](auto t)
                                                   {
                                                       using type = typename decltype(t)::type;

                                                       // if component already exists in components collection then we wont cache
                                                       if (!foundComponentsCollection->getComponent<type>())
                                                       {
                                                           auto entityComponentsList = entity->getComponents<type>();
                                                           for (const auto& component: entityComponentsList)
                                                           {
                                                               foundComponentsCollection->addComponent(component);
                                                           }
                                                       }
                                                   });

            auto f = m_cachedEntities.begin();

            // sorting layers by indices

            /*auto& foundComponentsCollection = m_cachedEntities[entity];
            foundComponentsCollection = foundComponentsCollection == nullptr ?
                                        std::make_shared<ComponentsCollection>() : foundComponentsCollection;

            Utils::Utils::forTypes<ComponentsT...>([&entity, &foundComponentsCollection](auto t)
                                                   {
                                                       using type = typename decltype(t)::type;

                                                       // if component already exists in components collection then we wont cache
                                                       if (!foundComponentsCollection->getComponent<type>())
                                                       {
                                                           auto entityComponentsList = entity->getComponents<type>();
                                                           for (const auto& component: entityComponentsList)
                                                           {
                                                               foundComponentsCollection->addComponent(component);
                                                           }
                                                       }
                                                   });*/

            double t1 = glfwGetTime();

            std::cout << "ms: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
        }

        bool in();

        const auto& getCachedEntities() const noexcept
        {
            return m_cachedEntities;
        }

        template<typename Func, typename... Args>
        void addFunctionToFixedUpdateQuery(const std::string& funcUUID, const Func& f, const Args&... args)
        {
            std::function<bool()> bindFunc = [f, args...]() { return f(args...); };
            m_fixedUpdateFunctionsQuery[funcUUID] = bindFunc;
        }

        template<typename Func, typename... Args>
        void addFunctionToUpdateQuery(const std::string& funcUUID, const Func& f, const Args&... args)
        {
            std::function<bool()> bindFunc = [f, args...]() { return f(args...); };
            m_updateFunctionsQuery[funcUUID] = (bindFunc);
        }

    protected:
        std::unordered_map<std::string, std::function<bool()>> m_fixedUpdateFunctionsQuery;
        std::unordered_map<std::string, std::function<bool()>> m_updateFunctionsQuery;

        std::map<std::shared_ptr<Layer>, std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<ComponentsCollection>>, LayersComparator> m_cachedEntities;
    };
}

#endif //NATIVECORE_ISYSTEM_H
