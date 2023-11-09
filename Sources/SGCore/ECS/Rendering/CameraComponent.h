#ifndef SUNGEARENGINE_CAMERACOMPONENT_H
#define SUNGEARENGINE_CAMERACOMPONENT_H

#include "SGCore/ECS/Transformations/Camera3DMovementSystem.h"
#include "SGCore/ImportedScenesArch/IMesh.h"

#include "IRenderingComponent.h"

namespace Core::ECS
{
    struct PostProcessLayer
    {
        std::shared_ptr<Graphics::IFrameBuffer> m_frameBuffer;
        std::string m_name = "default";
    };

    class CameraComponent : public IRenderingComponent
    {
        friend class Camera3DMovementSystem;

    public:
        CameraComponent();

        std::shared_ptr<ImportedScene::IMesh> m_billboard;

        // default frame buffer for layers that does not have post-processing
        std::shared_ptr<Graphics::IFrameBuffer> m_defaultLayersFrameBuffer;

        // final frame buffer with all post-processing
        std::shared_ptr<Graphics::IFrameBuffer> m_finalFrameBuffer;

        std::shared_ptr<Graphics::MarkedShader> m_postProcessQuadPassMarkedShader;

        // can be helpful for ImGUI
        bool m_useFinalFrameBuffer = false;

        std::shared_ptr<Graphics::IFrameBuffer> getPostProcessLayerFrameBuffer(const std::shared_ptr<ECS::Layer>& layer) noexcept;

        void addPostProcessLayer(const std::string& ppLayerName,
                                 const std::shared_ptr<ECS::Layer>& layer);

        void bindPostProcessLayers() const noexcept;

        // todo: make rename pp layer function

    private:
        std::unordered_map<std::shared_ptr<ECS::Layer>, PostProcessLayer> m_postProcessLayers;

        void init() noexcept final { }
    };
}

#endif //SUNGEARENGINE_CAMERACOMPONENT_H
