//
// Created by stuka on 24.11.2023.
//

#ifndef SUNGEARENGINE_IRENDERPASS_H
#define SUNGEARENGINE_IRENDERPASS_H

#include "SGCore/ECS/ComponentsCollector.h"
#include "SGCore/Graphics/API/ISubPassShader.h"
#include "SGUtils/Timer.h"
#include "SGECS/RegistryView.h"
#include "ICamera.h"

namespace SGCore
{
    struct IRenderPipeline;

    struct IRenderPass
    {
        bool m_active = true;

        // components, that use to render in
        // Ref<CollectorCachedEntities> m_componentsToRenderIn;
        // collector for components to render
        // Ref<CollectorCachedEntities> m_componentsToRender;

        virtual void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) = 0;
    };
}

#endif //SUNGEARENGINE_IRENDERPASS_H
