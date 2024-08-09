//
// Created by ilya on 19.04.24.
//

#ifndef SUNGEARENGINE_GRAPHICEFFECT_H
#define SUNGEARENGINE_GRAPHICEFFECT_H

#include "SGCore/Main/CoreGlobals.h"

#include "SGCore/Graphics/API/ISubPassShader.h"

namespace SGCore
{
    struct PostProcessLayer;
    
    struct PostProcessEffect
    {
        friend struct PostProcessLayer;
        
        std::string m_name;
        
        virtual void onAttachToLayer(const Ref<PostProcessLayer>& toLayer);
        virtual void onLayerShaderChanged(const Ref<PostProcessLayer>& layer);
        virtual void onFXPass(const Ref<PostProcessLayer>& currentLayer) { }
        virtual void onDetachFromLayer(const Ref<PostProcessLayer>& fromLayer) { }
        
        virtual void passValuesToSubPassShader(const Ref<ISubPassShader>& subPassShader) noexcept { }
        
        [[nodiscard]] bool isEnabled() const noexcept;
        void setEnabled(bool isEnabled) noexcept;
        
    protected:
        bool m_isEnabled = true;
        
        std::vector<Weak<PostProcessLayer>> m_parentPostProcessLayers;
    };
}

#endif // SUNGEARENGINE_GRAPHICEFFECT_H
