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
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IComponent;
    class Layer;
    class Scene;

    class Entity : public std::enable_shared_from_this<Entity>
    {
        friend class Scene;
        friend class Node;

    public:
        std::string m_name = "SGEntity";

        std::set<Ref<Entity>> m_children;

        void addComponent(const Ref<IComponent>&) noexcept;

        /**
         * Finds the first component of type ComponentT.
         * @tparam ComponentT - The type of component to find.
         * @return Found component
         */
        template<typename ComponentT>
        requires(std::is_base_of_v<IComponent, ComponentT>)
        Ref<ComponentT> getComponent()
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
        std::list<Ref<ComponentT>> getComponents()
        {
            std::list<Ref<ComponentT>> foundComponents;

            for(auto& component : m_components)
            {
                if(SG_INSTANCEOF(component.get(), ComponentT))
                {
                    foundComponents.push_back(std::static_pointer_cast<ComponentT>(component));
                }
            }

            return foundComponents;
        }

        Ref<Layer> getLayer() const noexcept;

        size_t getCountOfEntities(const std::string& entitiesNames) const noexcept;

        size_t getSceneSameNameIndex() const noexcept;

        // todo: make remove component and remove components of type

    private:
        // what index of same name in scene has this entity
        size_t m_sceneSameNameIndex = 0;

        std::list<Ref<IComponent>> m_components;

        // todo: make only index of layer.
        Weak<Layer> m_layer;
        Weak<Scene> m_scene;
    };
}

#endif //NATIVECORE_ENTITY_H