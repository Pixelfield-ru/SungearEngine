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

        Ref<IFrameBuffer> m_frameBuffer;

        Ref<IShader> m_shader;

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
        Ref<IMeshData> m_postProcessQuad;

        // passes
        Ref<IShader> m_defaultPostProcessShader;
        Ref<IShader> m_finalPostProcessOverlayShader;

        // default frame buffer for layers that does not have post-processing
        Ref<IFrameBuffer> m_defaultLayersFrameBuffer;
        // final frame buffer with all post-processing
        Ref<IFrameBuffer> m_finalFrameBuffer;

        // can be helpful for ImGUI
        bool m_useFinalFrameBuffer = false;

        Ref<IFrameBuffer> getPostProcessLayerFrameBuffer(const Ref<Layer>& layer) noexcept;

        void addPostProcessLayer(const std::string& ppLayerName,
                                 const Ref<Layer>& layer,
                                 const std::uint16_t& fbWidth,
                                 const std::uint16_t& fbHeight);

        void addPostProcessLayer(const std::string& ppLayerName,
                                 const Ref<Layer>& layer);

        void setPostProcessLayerShader(const Ref<Layer>& layer,
                                       const Ref<IShader>& shader) noexcept;

        void bindPostProcessLayers() noexcept;

        const auto& getPostProcessLayers() const noexcept
        {
            return m_postProcessLayers;
        }

        // todo: make rename pp layer function

    private:
        std::unordered_map<Ref<Layer>, PostProcessLayer> m_postProcessLayers;

        void init() noexcept final { }
    };
}

#endif //SUNGEARENGINE_CAMERACOMPONENT_H
