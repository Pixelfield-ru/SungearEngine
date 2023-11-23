//
// Created by stuka on 22.10.2023.
//

#ifndef SUNGEARENGINE_MESHEDENTITIESCOLLECTORSYSTEM_H
#define SUNGEARENGINE_MESHEDENTITIESCOLLECTORSYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "SGCore/Patterns/Singleton.h"

namespace SGCore
{
    struct MeshesCollector : public ISystem
    {
        SG_CUSTOM_CTOR_SINGLETON(MeshesCollector)
        SG_COPY_SINGLETON(MeshesCollector)
        SG_MOVE_SINGLETON(MeshesCollector)
    };
}

#endif //SUNGEARENGINE_MESHEDENTITIESCOLLECTORSYSTEM_H
