//
// Created by Ilya on 03.11.2023.
//

#ifndef SUNGEARENGINE_IPIPELINESYSTEM_H
#define SUNGEARENGINE_IPIPELINESYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "SGCore/Graphics/API/MarkedShader.h"

namespace Core::ECS
{
    struct IRenderPipeline : public ISystem
    {
    public:
        std::shared_ptr<Graphics::MarkedShader> m_shadowsPassMarkedShader;
        std::shared_ptr<Graphics::MarkedShader> m_geometryPassMarkedShader;
        std::shared_ptr<Graphics::MarkedShader> m_skyboxPassMarkedShader;

        // primitives pass
        std::shared_ptr<Graphics::MarkedShader> m_linesPassMarkedShader;
        std::shared_ptr<Graphics::MarkedShader> m_complexPrimitivesPassMarkedShader;

        // IPipelineSystem();
    };
}

#endif // SUNGEARENGINE_IPIPELINESYSTEM_H
