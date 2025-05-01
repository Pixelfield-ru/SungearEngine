#ifndef SUNGEARENGINE_CAMERACOMPONENT_H
#define SUNGEARENGINE_CAMERACOMPONENT_H

#include <SGCore/pch.h>

#include "SGCore/Render/PostProcess/PostProcessEffect.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Render/PostProcess/PostProcessFXSubPass.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Graphics/API/RenderState.h"

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

        std::string m_name;
        
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
        
        void setFXSubPassShader(const AssetRef<IShader>& FXSubPassShader) noexcept
        {
            m_FXSubPassShader = FXSubPassShader;
            
            for(const auto& effect : m_effects)
            {
                effect->onLayerShaderChanged(shared_from_this());
            }
        }

        const std::uint16_t& getIndex() const noexcept
        {
            return m_index;
        }
        
    private:
        std::uint16_t m_index = 0;

        AssetRef<IShader> m_FXSubPassShader;
        
        std::vector<Ref<PostProcessEffect>> m_effects;
    };

    // todo: make change for default PP shader
    class LayeredFrameReceiver : public ECS::Component<LayeredFrameReceiver, const LayeredFrameReceiver>
    {
        friend struct Controllables3DUpdater;

    public:
        LayeredFrameReceiver();

        Ref<IShader> m_shader;

        // CONTAINS ONLY COLOR0 WITH VALUES OF LAYERS AND COLOR1 WITH COLORS OF LAYERS
        Ref<IFrameBuffer> m_layersFrameBuffer;
        // CONTAINS OUTPUT FX
        Ref<IFrameBuffer> m_layersFXFrameBuffer;
        // ATTACHMENT THAT ARE USED TO RENDER IN THEM.
        std::set<SGFrameBufferAttachmentType> m_attachmentToRenderIn { SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0,
                                                                       SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1,
                                                                       SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2,
                                                                       SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3,
                                                                       SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT4 };
        
        Ref<PostProcessLayer> addOrGetLayer(const std::string& name,
                                            const std::uint16_t& fbWidth,
                                            const std::uint16_t& fbHeight);
        
        Ref<PostProcessLayer> addLayer(const std::string& name);

        void removeLayer(const std::string& name) noexcept;

        [[nodiscard]] const auto& getLayers() const noexcept
        {
            return m_layers;
        }
        
        Ref<PostProcessLayer> getLayer(const std::string& name) noexcept;
        Ref<PostProcessLayer> getDefaultLayer() const noexcept;
        
        [[nodiscard]] std::uint16_t getLayersMaximumIndex() const noexcept;
        
        void clearPostProcessFrameBuffers() const noexcept;
        
    private:
        MeshRenderState m_quadMeshRenderState;
        Ref<IMeshData> m_postProcessQuad;

        Ref<PostProcessLayer> m_defaultLayer;
        
        std::vector<Ref<PostProcessLayer>> m_layers;
    };
}

#endif //SUNGEARENGINE_CAMERACOMPONENT_H
