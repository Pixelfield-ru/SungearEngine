#ifndef SUNGEARENGINE_CAMERACOMPONENT_H
#define SUNGEARENGINE_CAMERACOMPONENT_H

#include "SGCore/ECS/Transformations/CameraMovement3DSystem.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"

#include "IRenderingComponent.h"

namespace SGCore
{
    struct PostProcessLayer
    {
        friend class Camera;

        std::shared_ptr<IFrameBuffer> m_frameBuffer;

        std::shared_ptr<IShader> m_shader;

        // name just for user. for convenience
        std::string m_name = "default";

        std::uint16_t m_index = 0;

    private:
        // technical name
        std::string m_nameInShader = "allFB[0]";
    };

    // todo: make change for default PP shader
    class Camera : public IRenderingComponent
    {
        friend class CameraMovement3DSystem;

    public:
        Camera();

        ShaderMarkup m_postProcessShadersMarkup;

        MeshDataRenderInfo m_postProcessQuadRenderInfo;
        std::shared_ptr<IMeshData> m_postProcessQuad;

        // passes
        std::shared_ptr<IShader> m_defaultPostProcessShader;
        std::shared_ptr<IShader> m_finalPostProcessOverlayShader;

        // default frame buffer for layers that does not have post-processing
        std::shared_ptr<IFrameBuffer> m_defaultLayersFrameBuffer;
        // final frame buffer with all post-processing
        std::shared_ptr<IFrameBuffer> m_finalFrameBuffer;

        // can be helpful for ImGUI
        bool m_useFinalFrameBuffer = false;

        std::shared_ptr<IFrameBuffer> getPostProcessLayerFrameBuffer(const std::shared_ptr<Layer>& layer) noexcept;

        void addPostProcessLayer(const std::string& ppLayerName,
                                 const std::shared_ptr<Layer>& layer,
                                 const std::uint16_t& fbWidth,
                                 const std::uint16_t& fbHeight);

        void addPostProcessLayer(const std::string& ppLayerName,
                                 const std::shared_ptr<Layer>& layer);

        void setPostProcessLayerShader(const std::shared_ptr<Layer>& layer,
                                       const std::shared_ptr<IShader>& shader) noexcept;

        void bindPostProcessLayers() noexcept;

        const auto& getPostProcessLayers() const noexcept
        {
            return m_postProcessLayers;
        }

        // todo: make rename pp layer function

    private:
        std::unordered_map<std::shared_ptr<Layer>, PostProcessLayer> m_postProcessLayers;

        void init() noexcept final { }
    };
}

#endif //SUNGEARENGINE_CAMERACOMPONENT_H
