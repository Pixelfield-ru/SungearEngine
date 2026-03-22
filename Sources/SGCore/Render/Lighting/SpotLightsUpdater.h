//
// Created by ilya on 14.02.24.
//

#pragma once

#include "SGCore/Utils/Timer.h"
#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    class IUniformBuffer;
    
    struct SGCORE_EXPORT SpotLightsUpdater : public ISystem
    {
        sg_implement_type_id(SGCore::SpotLightsUpdater)

        SpotLightsUpdater() noexcept;
        
        void update(double dt, double fixedDt) final;
        
        // uniform buffer for directional lights
        Ref<IUniformBuffer> m_uniformBuffer;
        
        int m_maxLightsCount = 5;
        
        Timer m_lightsUpdateTimer;
        
    private:
        void updateLights() noexcept;

        Slot<void(double, double)> m_lightsUpdateEventListener = [this](auto, auto) {
            updateLights();
        };
    };
}
