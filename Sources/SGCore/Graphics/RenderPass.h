//
// Created by Ilya on 30.10.2023.
//

#ifndef SUNGEARENGINE_RENDERPASS_H
#define SUNGEARENGINE_RENDERPASS_H

#include "SGCore/ECS/Layer.h"
#include "API/IFrameBuffer.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/ImportedScenesArch/IMesh.h"

namespace Core::Graphics
{
    struct PostProcessLayer
    {
        std::shared_ptr<ECS::Layer> m_renderingLayer;

        std::shared_ptr<IFrameBuffer> m_frameBuffer;
        std::shared_ptr<IShader> m_shader;
        std::shared_ptr<ImportedScene::IMesh> m_billboard;
    };

    class RenderPass
    {
        std::list<PostProcessLayer> m_postProcessLayers;

        RenderPass();
    };
}

#endif // SUNGEARENGINE_RENDERPASS_H
