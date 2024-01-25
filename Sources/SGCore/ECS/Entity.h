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

#include "SGUtils/Utils.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGUtils/UniqueName.h"

namespace SGCore
{
    class IComponent;
    class Layer;
    class Scene;

    class Entity : public SGUtils::UniqueNameWrapper, public std::enable_shared_from_this<Entity>
    {
        friend class Scene;
        friend class Node;

    public:
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

        void setParentScene(const Ref<Scene>& scene) noexcept;

        const auto& getChildren() const noexcept
        {
            return m_children;
        }

        void addChild(const Ref<Entity>& child) noexcept;
        void removeChild(const Ref<Entity>& child) noexcept;
        void clearChildren() noexcept;

        // todo: make remove component and remove components of type

    private:
        std::list<Ref<IComponent>> m_components;

        std::set<Ref<Entity>> m_children;

        // todo: make only index of layer.
        Weak<Layer> m_layer;
        Weak<Scene> m_parentScene;
    };
}

#endif //NATIVECORE_ENTITY_H