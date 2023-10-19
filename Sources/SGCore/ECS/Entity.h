//
// Created by stuka on 02.05.2023.
//

#ifndef NATIVECORE_ENTITY_H
#define NATIVECORE_ENTITY_H

#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <tsl/robin_map.h>

/*#include "SGCore/ECS/Rendering/Lighting/ShadowsCasterComponent.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightComponent.h"

#include "ECSWorld.h"*/

#include "IComponent.h"
#include "SGCore/Utils/Utils.h"
#include "Scene.h"

namespace Core::ECS
{
    class IComponent;
    class ShadowsCasterComponent;
    class DirectionalLightComponent;

    class Entity
    {
    public:
        std::string m_name;

        std::set<std::shared_ptr<Entity>> m_children;

        template<class ComponentT>
        requires(std::is_base_of_v<IComponent, ComponentT>)
        void addComponent(const std::shared_ptr<ComponentT>& component) noexcept
        {
            const size_t componentTID = typeid(ComponentT).hash_code();

            std::cout << typeid(ComponentT).name() << std::endl;

            m_components[componentTID].push_back(component);

            if(SG_INSTANCEOF(component.get(), ShadowsCasterComponent))
            {
                if(Scene::getCurrentScene())
                {
                    // todo: do for scene where entity is placed
                    Scene::getCurrentScene()->setShadowsCastersNum(Scene::getCurrentScene()->getShadowsCastersNum() + 1);
                }
            }

            if(SG_INSTANCEOF(component.get(), DirectionalLightComponent))
            {
                if(Scene::getCurrentScene())
                {
                    // todo: do for scene where entity is placed
                    Scene::getCurrentScene()->setDirectionalLightsNum(
                            Scene::getCurrentScene()->getDirectionalLightsNum() + 1
                    );
                }
            }
        }

        /**
         * Finds the first component of type ComponentT.
         * @tparam ComponentT - The type of component to find.
         * @return Found component
         */
        template<typename ComponentT>
        requires(std::is_base_of_v<IComponent, ComponentT>)
        std::shared_ptr<ComponentT> getComponent()
        {
            const size_t componentTID = typeid(ComponentT).hash_code();
            const auto& componentsIter = m_components.find(componentTID);
            std::shared_ptr<ComponentT> retComponent = (componentsIter != m_components.cend() && !componentsIter->second.empty()) ? std::dynamic_pointer_cast<ComponentT>(*componentsIter->second.begin()) : nullptr;
            return retComponent;
        }

        /**
         * Finds all components of type ComponentT.
         * @tparam ComponentT - The type of component to find.
         * @return Found component
         */
        template<typename ComponentT>
        requires(std::is_base_of_v<IComponent, ComponentT>)
        std::list<std::shared_ptr<ComponentT>> getComponents()
        {
            // TODO: make pattern class with func size_t getHashCode()
            const size_t componentTID = typeid(ComponentT).hash_code();
            const auto& typedComponents = m_components[componentTID];

            std::list<std::shared_ptr<ComponentT>> retComponents;

            for(const auto& component : typedComponents)
            {
                retComponents.push_back(std::dynamic_pointer_cast<ComponentT>(component));
            }

            return retComponents;
        }

        // todo: make remove component and remove components of type

    private:
        // first - typeid of Component
        // second - list of this components
        tsl::robin_map<size_t, std::list<std::shared_ptr<IComponent>>> m_components;
    };
}

#endif //NATIVECORE_ENTITY_H
