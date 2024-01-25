//
// Created by Ilya on 25.11.2023.
//

#ifndef SUNGEARENGINE_CAMERASCOLLECTOR_H
#define SUNGEARENGINE_CAMERASCOLLECTOR_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    struct CamerasCollector : public ISystem
    {
        CamerasCollector();
        SG_COPY_CTOR(CamerasCollector);
        SG_MOVE_CTOR(CamerasCollector);
    };
}

#endif //SUNGEARENGINE_CAMERASCOLLECTOR_H
