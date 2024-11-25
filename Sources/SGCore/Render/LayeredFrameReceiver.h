#ifndef SUNGEARENGINE_CAMERACOMPONENT_H
#define SUNGEARENGINE_CAMERACOMPONENT_H

#include <SGCore/pch.h>

#include "SGCore/Render/PostProcess/PostProcessEffect.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Render/PostProcess/PostProcessFXSubPass.h"
#include "SGCore/Scene/Layer.h"
#include "SGCore/Utils/EventListener.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"

#define SG_PP_LAYER_FB_NAME(idx)  ("frameBuffer" + std::to_string(idx))

namespace SGCore
{
    class IFrameBuffer;

    struct FrameBufferAttachmentWithBlock
    {
        SGFrameBufferAttachmentType m_frameBufferAttachmentType = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0;
    };

    struct PostProcessLayer : public std::enable_shared_from_this<PostProcessLayer>
    {
        friend class LayeredFrameReceiver;

        std::vector<PostProcessFXSubPass> m_subPasses;

        Ref<IFrameBuffer> m_frameBuffer;

        std::string m_name;

        std::uint16_t m_index = 0;
        
        // attachments that the scene will be rendered into
        std::vector<SGFrameBufferAttachmentType> m_attachmentsToRenderIn { SGG_COLOR_ATTACHMENT0 };

        // first - to which attachment of the output buffer will the data from the attachment "second" be copied
        // second - the attachment to be copied
        std::unordered_map<SGFrameBufferAttachmentType, SGFrameBufferAttachmentType> m_attachmentsForCombining {
                { SGG_COLOR_ATTACHMENT1, SGG_COLOR_ATTACHMENT0 }/*,
                { SGG_COLOR_ATTACHMENT1, SGG_COLOR_ATTACHMENT2 },
                { SGG_COLOR_ATTACHMENT2, SGG_COLOR_ATTACHMENT3 },
                { SGG_COLOR_ATTACHMENT3, SGG_COLOR_ATTACHMENT4 },
                { SGG_COLOR_ATTACHMENT4, SGG_COLOR_ATTACHMENT5 },
                { SGG_COLOR_ATTACHMENT5, SGG_COLOR_ATTACHMENT6 }*/
        };
        
        template<typename EffectT>
        requires(std::is_base_of_v<PostProcessEffect, EffectT>)
        void addEffect(const Ref<EffectT>& effect) noexcept
        {
            if(getEffect<EffectT>()) return;
            
            m_effects.push_back(effect);
            
            auto thisShared = shared_from_this();
            
            effect->m_parentPostProcessLayers.push_back(thisShared);
            effect->onAttachToLayer(thisShared);
        }
        
        template<typename EffectT>
        requires(std::is_base_of_v<PostProcessEffect, EffectT>)
        void removeEffect() noexcept
        {
            std::erase_if(m_effects, [this](const Ref<PostProcessEffect>& otherEffect) {
                if(SG_INSTANCEOF(otherEffect.get(), EffectT))
                {
                    auto thisShared = shared_from_this();
                    
                    std::erase_if(otherEffect->m_parentPostProcessLayers, [this](const Weak<PostProcessLayer>& parentLayer) {
                        auto lockedLayer = parentLayer.lock();
                        return lockedLayer && lockedLayer.get() == this;
                    });
                    
                    otherEffect->onDetachFromLayer(thisShared);
                    
                    return true;
                }
                
                return false;
            });
        }
        
        template<typename EffectT>
        requires(std::is_base_of_v<PostProcessEffect, EffectT>)
        Ref<EffectT> getEffect() const noexcept
        {
            for(const auto& effect : m_effects)
            {
                if(SG_INSTANCEOF(effect.get(), EffectT))
                {
                    return std::static_pointer_cast<EffectT>(effect);
                }
            }
            
            return nullptr;
        }
        
        [[nodiscard]] const auto& getEffects() const noexcept
        {
            return m_effects;
        }
        
        auto getFXSubPassShader() const noexcept
        {
            return m_FXSubPassShader;
        }
        
        void setFXSubPassShader(const Ref<ISubPassShader>& FXSubPassShader) noexcept
        {
            m_FXSubPassShader = FXSubPassShader;
            
            for(const auto& effect : m_effects)
            {
                effect->onLayerShaderChanged(shared_from_this());
            }
        }
        
    private:
        Ref<ISubPassShader> m_FXSubPassShader;
        
        std::vector<Ref<PostProcessEffect>> m_effects;
    };

    // todo: make change for default PP shader
    class LayeredFrameReceiver
    {
        friend struct Controllables3DUpdater;

    public:
        LayeredFrameReceiver();

        // contains only PostProcessFinalFXPass
        Ref<ISubPassShader> m_finalFrameFXShader;
        
        // contains depth pass and combination pass
        Ref<IShader> m_shader;

        Ref<IFrameBuffer> m_layersCombinedBuffer;

        // final frame buffer with all post-processing
        Ref<IFrameBuffer> m_finalFrameFXFrameBuffer;
        
        Ref<PostProcessLayer> addOrGetLayer(const std::string& name,
                                            const std::uint16_t& fbWidth,
                                            const std::uint16_t& fbHeight);
        
        Ref<PostProcessLayer> addLayer(const std::string& name);

        void removeLayer(const std::string& name) noexcept;
        
        void setLayerShader(const std::string& name,
                            const Ref<ISubPassShader>& shader) noexcept;

        [[nodiscard]] const auto& getLayers() const noexcept
        {
            return m_layers;
        }
        
        Ref<PostProcessLayer> getLayer(const std::string& name) noexcept;
        Ref<PostProcessLayer> getDefaultLayer() noexcept;
        
        [[nodiscard]] std::uint16_t getLayersMaximumIndex() const noexcept;
        
        void clearPostProcessFrameBuffers() const noexcept;
        
        void attachmentDepthPass(const SGCore::Ref<SGCore::PostProcessLayer>& layer, SGFrameBufferAttachmentType attachmentType) noexcept;

        void setRenderOverlayInSeparateFrameBuffer(bool renderOverlayInSeparateFrameBuffer) noexcept;
        [[nodiscard]] bool isRenderOverlayInSeparateFrameBuffer() const noexcept;

        [[nodiscard]] Ref<IFrameBuffer> getOverlayFrameBuffer() const noexcept;
        
    private:
        MeshDataRenderInfo m_postProcessQuadRenderInfo;
        Ref<IMeshData> m_postProcessQuad;

        bool m_renderOverlayInSeparateFrameBuffer = false;
        // may be nullptr. this frame buffer has only one attachment (COLOR). not nullptr if m_renderOverlayInSeparateFrameBuffer == true
        Ref<IFrameBuffer> m_overlayFrameBuffer;

        Ref<PostProcessLayer> m_defaultLayer;
        
        std::vector<Ref<PostProcessLayer>> m_layers;
    };
}

#endif //SUNGEARENGINE_CAMERACOMPONENT_H
