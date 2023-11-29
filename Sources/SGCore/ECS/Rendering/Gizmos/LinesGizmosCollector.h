//
// Created by Ilya on 06.11.2023.
//

#ifndef SUNGEARENGINE_LINESCOLLECTORSYSTEM_H
#define SUNGEARENGINE_LINESCOLLECTORSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    struct LinesGizmosCollector : public ISystem
    {
        LinesGizmosCollector();
        SG_COPY_CTOR(LinesGizmosCollector);
        SG_MOVE_CTOR(LinesGizmosCollector);
    };
}

#endif // SUNGEARENGINE_LINESCOLLECTORSYSTEM_H
