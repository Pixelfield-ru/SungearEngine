//
// Created by Ilya on 06.11.2023.
//

#ifndef SUNGEARENGINE_COMPLEXPRIMITIVESCOLLECTORSYSTEM_H
#define SUNGEARENGINE_COMPLEXPRIMITIVESCOLLECTORSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace Core::ECS
{
    class ComplexPrimitivesCollectorSystem : public ISystem
    {
    public:
        void cacheEntity(const std::shared_ptr<Entity>& entity) final;
    };
}

#endif // SUNGEARENGINE_COMPLEXPRIMITIVESCOLLECTORSYSTEM_H
