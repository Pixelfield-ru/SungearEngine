//
// Created by Ilya on 30.10.2023.
//

#ifndef SUNGEARENGINE_RENDERPASS_H
#define SUNGEARENGINE_RENDERPASS_H

#include <memory>
#include <unordered_map>

namespace Core::ECS
{
    class Layer;
}

namespace Core::ImportedScene
{
    class IMesh;
}

namespace Core::Graphics
{
    class IFrameBuffer;

    struct RenderOutput
    {
        std::shared_ptr<ImportedScene::IMesh> m_billboard;

        // output frame buffer
        std::shared_ptr<IFrameBuffer> m_frameBuffer;

        RenderOutput();
    };

    struct RenderPass
    {
        std::unordered_map<std::shared_ptr<ECS::Layer>, RenderOutput> m_postProcessLayers;

        RenderOutput m_defaultRenderOutput;
    };
}

#endif // SUNGEARENGINE_RENDERPASS_H
