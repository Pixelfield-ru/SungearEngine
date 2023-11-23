//
// Created by Ilya on 03.11.2023.
//

#ifndef SUNGEARENGINE_IPIPELINESYSTEM_H
#define SUNGEARENGINE_IPIPELINESYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "SGCore/Graphics/API/ShaderMarkup.h"
#include "RenderPass.h"

namespace SGCore
{
    class IShader;

    struct IRenderPipeline : public ISystem
    {
        Ref<IShader> m_shadowsPassShader;
        ShaderMarkup m_shadowsPassShaderMarkup;

        Ref<IShader> m_geometryPassShader;
        ShaderMarkup m_geometryPassShaderMarkup;

        Ref<IShader> m_skyboxPassShader;
        ShaderMarkup m_skyboxPassShaderMarkup;

        // gizmos pass -----------------------
        Ref<IShader> m_linesGizmosPassShader;

        Ref<IShader> m_complexGizmosPassShader;

        // ------------------------------------

        std::vector<RenderPass> m_renderPasses;

        // IPipelineSystem();
    };
}

#endif // SUNGEARENGINE_IPIPELINESYSTEM_H
