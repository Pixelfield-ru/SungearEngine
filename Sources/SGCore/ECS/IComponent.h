//
// Created by stuka on 02.05.2023.
//

#ifndef NATIVECORE_COMPONENT_H
#define NATIVECORE_COMPONENT_H

#include <memory>

#include "Entity.h"
#include "SGCore/Patterns/UUID.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class Entity;

    class IComponent : public UUID
    {
        // just for polymorphism
        virtual void init() = 0;

    protected:
        Ref<Entity> entity;
    };
}

#endif //NATIVECORE_COMPONENT_H
