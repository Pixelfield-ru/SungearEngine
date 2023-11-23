//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_SHADOWSCASTERSYSTEM_H
#define SUNGEARENGINE_SHADOWSCASTERSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    class ShadowsCastersCollector : public ISystem
    {
        SG_CUSTOM_CTOR_SINGLETON(ShadowsCastersCollector)
        SG_COPY_SINGLETON(ShadowsCastersCollector)
        SG_MOVE_SINGLETON(ShadowsCastersCollector)
    };
}

#endif //SUNGEARENGINE_SHADOWSCASTERSYSTEM_H
