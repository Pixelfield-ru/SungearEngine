//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_SHADOWSCASTERSYSTEM_H
#define SUNGEARENGINE_SHADOWSCASTERSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace Core::ECS
{
    class ShadowsCastersCollector : public ISystem
    {
        SG_DECLARE_SINGLETON(ShadowsCastersCollector)

    public:
        void cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity) final;
    };
}

#endif //SUNGEARENGINE_SHADOWSCASTERSYSTEM_H
