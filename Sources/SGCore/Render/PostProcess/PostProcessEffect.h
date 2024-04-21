//
// Created by ilya on 19.04.24.
//

#ifndef SUNGEARENGINE_GRAPHICEFFECT_H
#define SUNGEARENGINE_GRAPHICEFFECT_H

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct PostProcessLayer;
    
    struct PostProcessEffect
    {
        friend struct PostProcessLayer;
        
        virtual void onAttachToLayer(const Ref<PostProcessLayer>& toLayer) { }
        virtual void onLayerShaderChanged(const Ref<PostProcessLayer>& layer) { }
        virtual void onFXPass(const Ref<PostProcessLayer>& currentLayer) { }
        virtual void onDetachFromLayer(const Ref<PostProcessLayer>& fromLayer) { }
        
    protected:
        std::vector<Weak<PostProcessLayer>> m_parentPostProcessLayers;
    };
}

#endif // SUNGEARENGINE_GRAPHICEFFECT_H
