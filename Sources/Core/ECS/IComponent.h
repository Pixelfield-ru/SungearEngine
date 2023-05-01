//
// Created by stuka on 02.05.2023.
//

#pragma once

#ifndef NATIVECORE_COMPONENT_H
#define NATIVECORE_COMPONENT_H

#include <memory>

#include "Entity.h"

namespace Core::ECS
{
    class IComponent
    {
        friend class Core::ECS::Entity;

    private:
        std::shared_ptr<Core::ECS::Entity> entity;
    };
}

#endif //NATIVECORE_COMPONENT_H
