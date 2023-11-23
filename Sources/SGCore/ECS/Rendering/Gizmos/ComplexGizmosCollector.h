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
        SG_CUSTOM_CTOR_SINGLETON(ComplexGizmosCollector)
        SG_COPY_SINGLETON(ComplexGizmosCollector)
        SG_MOVE_SINGLETON(ComplexGizmosCollector)
    };
}

#endif // SUNGEARENGINE_COMPLEXPRIMITIVESCOLLECTORSYSTEM_H
