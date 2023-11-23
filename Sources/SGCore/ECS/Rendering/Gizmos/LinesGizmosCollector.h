//
// Created by Ilya on 06.11.2023.
//

#ifndef SUNGEARENGINE_LINESCOLLECTORSYSTEM_H
#define SUNGEARENGINE_LINESCOLLECTORSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    class LinesGizmosCollector : public ISystem
    {
        SG_CUSTOM_CTOR_SINGLETON(LinesGizmosCollector)
        SG_COPY_SINGLETON(LinesGizmosCollector)
        SG_MOVE_SINGLETON(LinesGizmosCollector)
    };
}

#endif // SUNGEARENGINE_LINESCOLLECTORSYSTEM_H
