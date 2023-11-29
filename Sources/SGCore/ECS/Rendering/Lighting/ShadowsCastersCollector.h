//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_SHADOWSCASTERSYSTEM_H
#define SUNGEARENGINE_SHADOWSCASTERSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    struct ShadowsCastersCollector : public ISystem
    {
        ShadowsCastersCollector();
        SG_COPY_CTOR(ShadowsCastersCollector);
        SG_MOVE_CTOR(ShadowsCastersCollector);
    };
}

#endif //SUNGEARENGINE_SHADOWSCASTERSYSTEM_H
