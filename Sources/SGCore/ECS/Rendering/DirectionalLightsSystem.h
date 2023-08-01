//
// Created by stuka on 01.08.2023.
//

#ifndef SUNGEARENGINE_DIRECTIONALLIGHTSSYSTEM_H
#define SUNGEARENGINE_DIRECTIONALLIGHTSSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace Core::ECS
{
    class DirectionalLightsSystem : public ISystem
    {
    public:
        void update(const std::shared_ptr<Scene>& scene) override;
    };
}

#endif //SUNGEARENGINE_DIRECTIONALLIGHTSSYSTEM_H
