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

    struct RenderPass
    {
        std::unordered_map<std::shared_ptr<ECS::Layer>, std::shared_ptr<IFrameBuffer>> m_postProcessLayers;

        std::shared_ptr<ImportedScene::IMesh> m_billboard;

        // default frame buffer for layers that does not have post-processing
        std::shared_ptr<IFrameBuffer> m_defaultLayerFrameBuffer;

        // final render pass frame buffer with all post-processing
        std::shared_ptr<IFrameBuffer> m_finalFrameBuffer;

        // can be helpful for ImGUI
        bool m_useFinalFrameBuffer = false;

        RenderPass();
    };
}

#endif // SUNGEARENGINE_RENDERPASS_H
