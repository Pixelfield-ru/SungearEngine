//
// Created by Ilya on 20.10.2023.
//

#ifndef SUNGEARENGINE_COMPONENTSCOLLECTION_H
#define SUNGEARENGINE_COMPONENTSCOLLECTION_H

#include <vector>
#include <memory>
#include <list>

#include "SGCore/Main/CoreGlobals.h"
#include "IComponent.h"
#include "SGUtils/Utils.h"

namespace SGCore
{
    class ComponentsCollection
    {
    private:
        std::vector<Ref<IComponent>> m_components;
    public:
        ComponentsCollection() = default;

        void clear() noexcept;

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
    };
}

#endif //SUNGEARENGINE_COMPONENTSCOLLECTION_H
