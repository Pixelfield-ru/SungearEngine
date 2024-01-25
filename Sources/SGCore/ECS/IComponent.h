//
// Created by stuka on 02.05.2023.
//

#ifndef NATIVECORE_COMPONENT_H
#define NATIVECORE_COMPONENT_H

#include <memory>

namespace SGCore
{
    class Entity;

    class IComponent
    {
        // just for polymorphism
        virtual void init() = 0;
    };
}

#endif //NATIVECORE_COMPONENT_H
