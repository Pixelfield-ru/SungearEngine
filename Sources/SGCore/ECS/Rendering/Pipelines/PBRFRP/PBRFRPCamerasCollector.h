//
// Created by Ilya on 25.11.2023.
//

#ifndef SUNGEARENGINE_PBRFRPCAMERASCOLLECTOR_H
#define SUNGEARENGINE_PBRFRPCAMERASCOLLECTOR_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    struct PBRFRPCamerasCollector : public ISystem
    {
        PBRFRPCamerasCollector();
        SG_COPY_CTOR(PBRFRPCamerasCollector);
        SG_MOVE_CTOR(PBRFRPCamerasCollector);
    };
}

#endif //SUNGEARENGINE_PBRFRPCAMERASCOLLECTOR_H
