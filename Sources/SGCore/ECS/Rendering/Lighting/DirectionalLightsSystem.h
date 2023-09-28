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
        void FPSRelativeFixedUpdate(const std::shared_ptr<Scene>& scene) final;
    };
}

#endif //SUNGEARENGINE_DIRECTIONALLIGHTSSYSTEM_H
