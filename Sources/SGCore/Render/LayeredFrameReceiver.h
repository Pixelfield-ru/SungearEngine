#ifndef SUNGEARENGINE_CAMERACOMPONENT_H
#define SUNGEARENGINE_CAMERACOMPONENT_H

#include <set>

#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Render/PostProcess/PostProcessFXSubPass.h"
#include "SGCore/Scene/Layer.h"
#include "SGUtils/EventListener.h"

#define SG_PP_LAYER_FB_NAME(idx)  ("frameBuffer" + std::to_string(idx))

namespace SGCore
{
    class IFrameBuffer;

    struct FrameBufferAttachmentWithBlock
    {
        SGFrameBufferAttachmentType m_frameBufferAttachmentType = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0;
    };

    struct PostProcessLayer
    {
        friend class LayeredFrameReceiver;

        std::vector<PostProcessFXSubPass> m_subPasses;

        Ref<IFrameBuffer> m_frameBuffer;

        std::string m_name;

        Ref<ISubPassShader> m_FXSubPassShader;

        std::uint16_t m_index = 0;
        
        // attachments that the scene will be rendered into
        std::vector<SGFrameBufferAttachmentType> m_attachmentsToRenderIn { SGG_COLOR_ATTACHMENT0, // BY DEFAULT DEPTH NON-TESTED ATTACHMENT
                                                                           SGG_COLOR_ATTACHMENT1/*, // BY DEFAULT DEPTH-TESTED ATTACHMENT
                                                                           SGG_COLOR_ATTACHMENT2,
                                                                           SGG_COLOR_ATTACHMENT3,
                                                                           SGG_COLOR_ATTACHMENT4,
                                                                           SGG_COLOR_ATTACHMENT5,
                                                                           SGG_COLOR_ATTACHMENT6*/};
        
        // attachments that will pass the depth test
        // SGG_COLOR_ATTACHMENT0 IS NOT DEPTH-TESTED ATTACHMENT
        std::vector<SGFrameBufferAttachmentType> m_attachmentsToDepthTest { SGG_COLOR_ATTACHMENT1/*,
                                                                            SGG_COLOR_ATTACHMENT2,
                                                                            SGG_COLOR_ATTACHMENT3,
                                                                            SGG_COLOR_ATTACHMENT4,
                                                                            SGG_COLOR_ATTACHMENT5,
                                                                            SGG_COLOR_ATTACHMENT6*/};

        // first - to which attachment of the output buffer will the data from the attachment "second" be copied
        // second - the attachment to be copied
        std::unordered_map<SGFrameBufferAttachmentType, SGFrameBufferAttachmentType> m_attachmentsForCombining {
                { SGG_COLOR_ATTACHMENT0, SGG_COLOR_ATTACHMENT1 }/*,
                { SGG_COLOR_ATTACHMENT1, SGG_COLOR_ATTACHMENT2 },
                { SGG_COLOR_ATTACHMENT2, SGG_COLOR_ATTACHMENT3 },
                { SGG_COLOR_ATTACHMENT3, SGG_COLOR_ATTACHMENT4 },
                { SGG_COLOR_ATTACHMENT4, SGG_COLOR_ATTACHMENT5 },
                { SGG_COLOR_ATTACHMENT5, SGG_COLOR_ATTACHMENT6 }*/
        };

        std::string getNameInShader() const noexcept
        {
            return m_nameInShader;
        }

    private:
        // technical name
        std::string m_nameInShader = SG_PP_LAYER_FB_NAME(0);
    };

    // todo: make change for default PP shader
    class LayeredFrameReceiver
    {
        friend struct Controllables3DUpdater;

    public:
        LayeredFrameReceiver();
        
        Ref<IShader> m_shader;

        Ref<IFrameBuffer> m_ppLayersCombinedBuffer;

        // final frame buffer with all post-processing
        Ref<IFrameBuffer> m_finalFrameFXFrameBuffer;

        // can be helpful for ImGUI
        bool m_useFinalFrameBuffer = false;

        std::set<SGFrameBufferAttachmentType> m_attachmentsForCombining;
        
        Ref<PostProcessLayer> addPostProcessLayer(const std::string& name,
                                                  const std::uint16_t& fbWidth,
                                                  const std::uint16_t& fbHeight);
        
        Ref<PostProcessLayer> addPostProcessLayer(const std::string& name);

        void setPostProcessLayerShader(const std::string& name,
                                       const Ref<ISubPassShader>& shader) noexcept;

        [[nodiscard]] const auto& getPostProcessLayers() const noexcept
        {
            return m_postProcessLayers;
        }
        
        Ref<PostProcessLayer> getPostProcessLayer(const std::string& name) noexcept;
        Ref<PostProcessLayer> getDefaultPostProcessLayer() noexcept;
        
        void bindPostProcessFrameBuffer(const Ref<Layer>& layer) noexcept;

        void unbindPostProcessFrameBuffer() const noexcept;

        void clearPostProcessFrameBuffers() const noexcept;

        // todo: make rename pp layer function

    private:
        Ref<PostProcessLayer> m_defaultLayer;
        
        std::vector<Ref<PostProcessLayer>> m_postProcessLayers;

        Ref<IFrameBuffer> m_currentPPFrameBufferToBind;
    };
}

#endif //SUNGEARENGINE_CAMERACOMPONENT_H
