//
// Created by Ilya on 25.11.2023.
//

#ifndef SUNGEARENGINE_CAMERASCOLLECTOR_H
#define SUNGEARENGINE_CAMERASCOLLECTOR_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    class CamerasCollector : public ISystem
    {
        SG_CUSTOM_CTOR_SINGLETON(CamerasCollector)
        SG_COPY_SINGLETON(CamerasCollector)
        SG_MOVE_SINGLETON(CamerasCollector)
    };
}

#endif //SUNGEARENGINE_CAMERASCOLLECTOR_H
