//
// Created by stuka on 02.05.2023.
//

#ifndef NATIVECORE_ENTITY_H
#define NATIVECORE_ENTITY_H

#include <iostream>
#include <list>
#include <memory>

#include "IComponent.h"

namespace Core::ECS
{
    class Entity
    {
    private:
        std::list<std::unique_ptr<IComponent>> components;

    public:
        void addComponent(const IComponent* component) noexcept
        {

        }
    };
}

#endif //NATIVECORE_ENTITY_H
