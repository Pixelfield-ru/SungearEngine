//
// Created by ilya on 14.02.24.
//

#ifndef SUNGEARENGINE_ATMOSPHERESCATTERINGUPDATER_H
#define SUNGEARENGINE_ATMOSPHERESCATTERINGUPDATER_H

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    class IUniformBuffer;
    
    struct AtmosphereScatteringUpdater : public ISystem
    {
        AtmosphereScatteringUpdater() noexcept;
        
        void fixedUpdate(const Ref<Scene>& scene) final;
        
    private:
        Ref<IUniformBuffer> m_uniformBuffer;
    };
}

#endif //SUNGEARENGINE_ATMOSPHERESCATTERINGUPDATER_H
