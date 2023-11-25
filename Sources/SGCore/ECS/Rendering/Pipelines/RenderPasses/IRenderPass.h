//
// Created by stuka on 24.11.2023.
//

#ifndef SUNGEARENGINE_IRENDERPASS_H
#define SUNGEARENGINE_IRENDERPASS_H

#include "SGCore/ECS/ComponentsCollector.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Utils/Utils.h"

namespace SGCore
{
    struct IRenderPass
    {
        Ref<IShader> m_shader;
        ShaderMarkup m_shaderMarkup;

        // components, that use to render in
        Ref<CollectorCachedEntities> m_componentsToRenderIn;
        // collector for components to render
        Ref<CollectorCachedEntities> m_componentsToRender;

        virtual void render() = 0;
    };
}

#endif //SUNGEARENGINE_IRENDERPASS_H
