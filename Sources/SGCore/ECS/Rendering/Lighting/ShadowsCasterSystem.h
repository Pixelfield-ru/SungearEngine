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
        SG_DECLARE_SINGLETON(ShadowsCasterSystem)

    public:
        void update(const std::shared_ptr<Scene>& scene) final;

        void cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity) const final;
    };
}

#endif //SUNGEARENGINE_SHADOWSCASTERSYSTEM_H
