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

#include "IComponent.h"
#include "SGCore/Utils/Utils.h"

namespace Core::ECS
{
    class IComponent;
    class Layer;

    class Entity : public std::enable_shared_from_this<Entity>
    {
        friend class Scene;

    public:
        std::string m_name;

        std::set<std::shared_ptr<Entity>> m_children;

        void addComponent(const std::shared_ptr<IComponent>&) noexcept;

        /**
         * Finds the first component of type ComponentT.
         * @tparam ComponentT - The type of component to find.
         * @return Found component
         */
        template<typename ComponentT>
        requires(std::is_base_of_v<IComponent, ComponentT>)
        std::shared_ptr<ComponentT> getComponent()
        {
            for(auto& component : m_components)
            {
                if(SG_INSTANCEOF(component.get(), ComponentT))
                {
                    return std::static_pointer_cast<ComponentT>(component);
                }
            }

            return nullptr;
        }

        /**
         * Finds all components of type ComponentT.
         * @tparam ComponentT - The type of components to find.
         * @return Found components
         */
        template<typename ComponentT>
        requires(std::is_base_of_v<IComponent, ComponentT>)
        std::list<std::shared_ptr<ComponentT>> getComponents()
        {
            std::list<std::shared_ptr<ComponentT>> foundComponents;

            for(auto& component : m_components)
            {
                if(SG_INSTANCEOF(component.get(), ComponentT))
                {
                    foundComponents.push_back(std::static_pointer_cast<ComponentT>(component));
                }
            }

            return foundComponents;
        }

        // todo: make remove component and remove components of type

    private:
        std::list<std::shared_ptr<IComponent>> m_components;
        std::weak_ptr<Layer> m_layer;
    };
}

#endif //NATIVECORE_ENTITY_H