//
// Created by ilya on 14.02.24.
//

#pragma once

#include "SGCore/Utils/Timer.h"
#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    class IUniformBuffer;
    
    struct SGCORE_EXPORT AtmosphereUpdater : public ISystem
    {
        sg_implement_type_id(SGCore::AtmosphereUpdater)

        AtmosphereUpdater() noexcept;
        
        void update(double dt, double fixedDt) final;
        
        Timer m_atmosphereUpdateTimer;
        
        Ref<IUniformBuffer> m_uniformBuffer;
        
    private:
        void updateAtmosphere() noexcept;

        Slot<void(double, double)> m_atmosphereUpdateEventListener = [this](auto, auto) {
            updateAtmosphere();
        };
    };
}
