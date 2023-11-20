//
// Created by Ilya on 06.11.2023.
//

#ifndef SUNGEARENGINE_COMPLEXPRIMITIVESCOLLECTORSYSTEM_H
#define SUNGEARENGINE_COMPLEXPRIMITIVESCOLLECTORSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    class ComplexGizmosCollector : public ISystem
    {
    public:
        void cacheEntity(const Ref<Entity>& entity) final;
    };
}

#endif // SUNGEARENGINE_COMPLEXPRIMITIVESCOLLECTORSYSTEM_H
