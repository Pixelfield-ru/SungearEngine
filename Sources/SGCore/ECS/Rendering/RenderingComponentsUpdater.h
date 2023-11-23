//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_RENDERINGCOMPONENTSSYSTEM_H
#define SUNGEARENGINE_RENDERINGCOMPONENTSSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    struct RenderingComponentsUpdater : public ISystem
    {
        void fixedUpdate(const Ref<Scene>& scene) final;

        SG_CUSTOM_CTOR_SINGLETON(RenderingComponentsUpdater)
        SG_COPY_SINGLETON(RenderingComponentsUpdater)
        SG_MOVE_SINGLETON(RenderingComponentsUpdater)
    };
}

#endif //SUNGEARENGINE_RENDERINGCOMPONENTSSYSTEM_H
