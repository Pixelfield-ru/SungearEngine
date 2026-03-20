//
// Created by ilya on 15.04.24.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    /**
     * System to update all AudioSource components of all entities on scene.\n
     * Updates position, direction and state of audio sources.\n
     * Uses SGCore::Transform to update position and direction.\n
     * Invokes SGCore::AudioSource::onStateChanged signal if audio source state has changed.
     */
    struct SGCORE_EXPORT AudioProcessor : public ISystem
    {
        sg_implement_type_id(AudioProcessor, 27)

        void update(const double& dt, const double& fixedDt) override;
    };
}
