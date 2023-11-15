//
// Created by Ilya on 03.11.2023.
//

#ifndef SUNGEARENGINE_IPIPELINESYSTEM_H
#define SUNGEARENGINE_IPIPELINESYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "SGCore/Graphics/API/ShaderMarkup.h"

namespace Core::Graphics
{
    class IShader;
}

namespace Core::ECS
{
    struct IRenderPipeline : public ISystem
    {
    public:
        std::shared_ptr<Graphics::IShader> m_shadowsPassShader;
        Graphics::ShaderMarkup m_shadowsPassShaderMarkup;

        std::shared_ptr<Graphics::IShader> m_geometryPassShader;
        Graphics::ShaderMarkup m_geometryPassShaderMarkup;

        std::shared_ptr<Graphics::IShader> m_skyboxPassShader;
        Graphics::ShaderMarkup m_skyboxPassShaderMarkup;

        // gizmos pass -----------------------
        std::shared_ptr<Graphics::IShader> m_linesGizmosPassShader;

        std::shared_ptr<Graphics::IShader> m_complexGizmosPassShader;

        // ------------------------------------

        // IPipelineSystem();
    };
}

#endif // SUNGEARENGINE_IPIPELINESYSTEM_H
