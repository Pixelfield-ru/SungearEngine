//
// Created by stuka on 01.08.2023.
//

#ifndef SUNGEARENGINE_DIRECTIONALLIGHTSSYSTEM_H
#define SUNGEARENGINE_DIRECTIONALLIGHTSSYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "DirectionalLight.h"

namespace Core::ECS
{
    class DirectionalLightsCollector : public ISystem
    {
        SG_DECLARE_SINGLETON(DirectionalLightsCollector)

    public:
        void cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity) final;
    };
}

#endif //SUNGEARENGINE_DIRECTIONALLIGHTSSYSTEM_H
