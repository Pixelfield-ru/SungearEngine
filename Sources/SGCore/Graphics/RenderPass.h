//
// Created by Ilya on 30.10.2023.
//

#ifndef SUNGEARENGINE_RENDERPASS_H
#define SUNGEARENGINE_RENDERPASS_H

#include "SGCore/ECS/Layer.h"
#include "API/IFrameBuffer.h"

namespace Core::Graphics
{
    struct PostProcessLayer
    {
        std::shared_ptr<ECS::Layer> m_renderingLayer;
        std::shared_ptr<IFrameBuffer> m_frameBuffer;
    };

    class RenderPass
    {
        std::list<PostProcessLayer> m_postProcessLayers;
    };
}

#endif // SUNGEARENGINE_RENDERPASS_H
