//
// Created by Ilya on 06.11.2023.
//

#ifndef SUNGEARENGINE_COMPLEXPRIMITIVESCOLLECTORSYSTEM_H
#define SUNGEARENGINE_COMPLEXPRIMITIVESCOLLECTORSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    struct ComplexGizmosCollector : public ISystem
    {
        ComplexGizmosCollector();
        SG_COPY_CTOR(ComplexGizmosCollector);
        SG_MOVE_CTOR(ComplexGizmosCollector);
    };
}

#endif // SUNGEARENGINE_COMPLEXPRIMITIVESCOLLECTORSYSTEM_H
