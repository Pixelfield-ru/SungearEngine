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
        RenderingComponentsUpdater();
        SG_COPY_CTOR(RenderingComponentsUpdater);
        SG_MOVE_CTOR(RenderingComponentsUpdater);

        void fixedUpdate(const Ref<Scene>& scene) final;
    };
}

#endif //SUNGEARENGINE_RENDERINGCOMPONENTSSYSTEM_H
