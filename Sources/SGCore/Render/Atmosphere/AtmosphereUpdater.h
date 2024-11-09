//
// Created by ilya on 14.02.24.
//

#ifndef SUNGEARENGINE_ATMOSPHEREUPDATER_H
#define SUNGEARENGINE_ATMOSPHEREUPDATER_H

#include "SGCore/Utils/Timer.h"
#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    class IUniformBuffer;
    
    struct AtmosphereUpdater : public ISystem
    {
        sg_implement_type_id(AtmosphereUpdater, 18)

        AtmosphereUpdater() noexcept;
        
        void update(const double& dt, const double& fixedDt) final;
        
        Timer m_atmosphereUpdateTimer;
        
        Ref<IUniformBuffer> m_uniformBuffer;
        
    private:
        EventListener<void(const double&, const double&)> m_atmosphereUpdateEventListener = [this](auto, auto) {
            updateAtmosphere();
        };
        
        void updateAtmosphere() noexcept;
    };
}

#endif //SUNGEARENGINE_ATMOSPHEREUPDATER_H
