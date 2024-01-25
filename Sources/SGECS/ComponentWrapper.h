//
// Created by stuka on 06.01.2024.
//

#ifndef ECS_COMPONENTWRAPPER_H
#define ECS_COMPONENTWRAPPER_H

#include <cstddef>

namespace SGECS
{
    template<typename T>
    struct ComponentWrapper
    {
        template<typename... Args>
        explicit ComponentWrapper(Args&& ... args) : m_component(std::forward<Args>(args)...)
        {
        }

        T m_component;
        size_t m_entityID = 0;
    };
}

#endif //ECS_COMPONENTWRAPPER_H
