//
// Created by stuka on 24.11.2023.
//

#ifndef SUNGEARENGINE_RENDERPASS_H
#define SUNGEARENGINE_RENDERPASS_H

#include "SGCore/ECS/ComponentsCollector.h"
#include "SGCore/Graphics/API/IShader.h"

namespace SGCore
{
    struct RenderPass
    {
        Ref<IShader> m_shader;
        ShaderMarkup m_shaderMarkup;

        // collector for components to render
        ComponentsCollector m_componentsCollector;
    };
}

#endif //SUNGEARENGINE_RENDERPASS_H
