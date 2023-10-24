//
// Created by stuka on 24.10.2023.
//

#ifndef SUNGEARENGINE_LAYER_H
#define SUNGEARENGINE_LAYER_H

#include <list>
#include "Entity.h"

namespace Core::ECS
{
    class Layer
    {
        friend class Scene;

    public:
        std::list<std::shared_ptr<Entity>> m_entities;

        Layer() = default;

        Layer(Layer&& layer)
        {
            m_name = std::move(layer.m_name);

            std::cout << "layer move ctor" << std::endl;
        }

        Layer(const Layer& layer) = default;

        Layer& operator=(const Layer& layer) = default;
        Layer& operator=(Layer&& layer) = default;

    private:
        std::string m_name;
    };
}

#endif //SUNGEARENGINE_LAYER_H
