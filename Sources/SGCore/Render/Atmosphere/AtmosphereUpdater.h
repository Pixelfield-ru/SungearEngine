//
// Created by ilya on 14.02.24.
//

#ifndef SUNGEARENGINE_ATMOSPHEREUPDATER_H
#define SUNGEARENGINE_ATMOSPHEREUPDATER_H

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    class IUniformBuffer;
    
    struct AtmosphereUpdater : public ISystem
    {
        AtmosphereUpdater() noexcept;
        
        void fixedUpdate(const Ref<Scene>& scene) final;
        
    private:
        Ref<IUniformBuffer> m_uniformBuffer;
    };
}

#endif //SUNGEARENGINE_ATMOSPHEREUPDATER_H
