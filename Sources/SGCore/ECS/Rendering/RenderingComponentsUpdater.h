//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_RENDERINGCOMPONENTSSYSTEM_H
#define SUNGEARENGINE_RENDERINGCOMPONENTSSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    class RenderingComponentsUpdater : public ISystem
    {
        SG_DECLARE_SINGLETON(RenderingComponentsUpdater)

    public:
        void fixedUpdate(const Ref<Scene>& scene) final;

        void cacheEntity(const Ref<Entity>& entity) final;
    };
}

#endif //SUNGEARENGINE_RENDERINGCOMPONENTSSYSTEM_H
