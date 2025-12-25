//
// Created by ilya on 14.02.24.
//

#ifndef SUNGEARENGINE_DIRECTIONALLIGHTSUPDATER_H
#define SUNGEARENGINE_DIRECTIONALLIGHTSUPDATER_H

#include "SGCore/Utils/Timer.h"
#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    class IUniformBuffer;
    
    struct DirectionalLightsUpdater : public ISystem
    {
        sg_implement_type_id(DirectionalLightsUpdater, 19)

        DirectionalLightsUpdater() noexcept;
        
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

#endif //SUNGEARENGINE_DIRECTIONALLIGHTSUPDATER_H
