//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_SHADOWSCASTERSYSTEM_H
#define SUNGEARENGINE_SHADOWSCASTERSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace Core::ECS
{
    class ShadowsCasterSystem : public ISystem
    {
    public:
        void update(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity) final;

        void deltaUpdate(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity,
                         const double& deltaTime) final;
    };
}

#endif //SUNGEARENGINE_SHADOWSCASTERSYSTEM_H
