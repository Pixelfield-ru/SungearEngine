//
// Created by ilya on 14.02.24.
//

#pragma once

#include "SGCore/Utils/Timer.h"
#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    class IUniformBuffer;
    
    struct SpotLightsUpdater : public ISystem
    {
        sg_implement_type_id(SpotLightsUpdater, 19)

        SpotLightsUpdater() noexcept;
        
        void update(const double& dt, const double& fixedDt) final;
        
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
