//
// Created by stuka on 03.08.2023.
//

#ifndef SUNGEARENGINE_SKYBOXRENDERINGSYSTEM_H
#define SUNGEARENGINE_SKYBOXRENDERINGSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace Core::ECS
{
    class SkyboxesCollectorSystem : public ISystem
    {
    public:
        void cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity) final;
    };
}

#endif //SUNGEARENGINE_SKYBOXRENDERINGSYSTEM_H
