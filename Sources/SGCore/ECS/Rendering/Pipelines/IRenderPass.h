//
// Created by stuka on 24.11.2023.
//

#ifndef SUNGEARENGINE_IRENDERPASS_H
#define SUNGEARENGINE_IRENDERPASS_H

#include "IRenderPipeline.h"
#include "SGCore/ECS/ComponentsCollector.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Utils/Timer.h"

namespace SGCore
{
    class TimerCallback;

    struct IRenderPass
    {
        bool m_active = true;

        Ref<IShader> m_shader;
        ShaderMarkup m_shaderMarkup;

        // components, that use to render in
        Ref<CollectorCachedEntities> m_componentsToRenderIn;
        // collector for components to render
        Ref<CollectorCachedEntities> m_componentsToRender;

        virtual void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) = 0;
    };
}

#endif //SUNGEARENGINE_IRENDERPASS_H
