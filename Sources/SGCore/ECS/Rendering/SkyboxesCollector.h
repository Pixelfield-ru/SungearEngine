//
// Created by stuka on 03.08.2023.
//

#ifndef SUNGEARENGINE_SKYBOXRENDERINGSYSTEM_H
#define SUNGEARENGINE_SKYBOXRENDERINGSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    class SkyboxesCollector : public ISystem
    {
    public:
        void cacheEntity(const Ref<Entity>& entity) final;
    };
}

#endif //SUNGEARENGINE_SKYBOXRENDERINGSYSTEM_H
