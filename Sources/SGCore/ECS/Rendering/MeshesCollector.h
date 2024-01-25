//
// Created by stuka on 22.10.2023.
//

#ifndef SUNGEARENGINE_MESHEDENTITIESCOLLECTORSYSTEM_H
#define SUNGEARENGINE_MESHEDENTITIESCOLLECTORSYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "SGUtils/Singleton.h"

namespace SGCore
{
    struct MeshesCollector : public ISystem
    {
        MeshesCollector();
        SG_COPY_CTOR(MeshesCollector);
        SG_MOVE_CTOR(MeshesCollector);
    };
}

#endif //SUNGEARENGINE_MESHEDENTITIESCOLLECTORSYSTEM_H
