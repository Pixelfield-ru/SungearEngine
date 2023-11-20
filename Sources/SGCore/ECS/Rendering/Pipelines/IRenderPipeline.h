//
// Created by Ilya on 03.11.2023.
//

#ifndef SUNGEARENGINE_IPIPELINESYSTEM_H
#define SUNGEARENGINE_IPIPELINESYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "SGCore/Graphics/API/ShaderMarkup.h"

namespace SGCore
{
    class IShader;

    struct IRenderPipeline : public ISystem
    {
    public:
        std::shared_ptr<IShader> m_shadowsPassShader;
        ShaderMarkup m_shadowsPassShaderMarkup;

        std::shared_ptr<IShader> m_geometryPassShader;
        ShaderMarkup m_geometryPassShaderMarkup;

        std::shared_ptr<IShader> m_skyboxPassShader;
        ShaderMarkup m_skyboxPassShaderMarkup;

        // gizmos pass -----------------------
        std::shared_ptr<IShader> m_linesGizmosPassShader;

        std::shared_ptr<IShader> m_complexGizmosPassShader;

        // ------------------------------------

        // IPipelineSystem();
    };
}

#endif // SUNGEARENGINE_IPIPELINESYSTEM_H
