//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_RENDERINGCOMPONENTSSYSTEM_H
#define SUNGEARENGINE_RENDERINGCOMPONENTSSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace Core::ECS
{
    class RenderingComponentsSystem : public ISystem
    {
        SG_DECLARE_SINGLETON(RenderingComponentsSystem)

    public:
        void fixedUpdate(const std::shared_ptr<Scene>& scene) final;

        void cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity) const final;
    };
}

#endif //SUNGEARENGINE_RENDERINGCOMPONENTSSYSTEM_H
