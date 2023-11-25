//
// Created by Ilya on 03.11.2023.
//

#ifndef SUNGEARENGINE_IPIPELINESYSTEM_H
#define SUNGEARENGINE_IPIPELINESYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "SGCore/Graphics/API/ShaderMarkup.h"
#include "SGCore/ECS/Rendering/Pipelines/RenderPasses/IRenderPass.h"

namespace SGCore
{
    class IShader;

    struct IRenderPipeline : public ISystem
    {
        Ref<IShader> m_shadowsPassShader;
        ShaderMarkup m_shadowsPassShaderMarkup;

        Ref<IShader> m_geometryPassShader;
        ShaderMarkup m_geometryPassShaderMarkup;

        // gizmos pass -----------------------
        Ref<IShader> m_linesGizmosPassShader;

        Ref<IShader> m_complexGizmosPassShader;

        // ------------------------------------

        std::vector<Ref<IRenderPass>> m_renderPasses;

        std::function<void()> m_prepareFunc;

        // IPipelineSystem();
    };
}

#endif // SUNGEARENGINE_IPIPELINESYSTEM_H
