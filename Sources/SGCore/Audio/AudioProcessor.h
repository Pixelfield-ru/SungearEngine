//
// Created by ilya on 15.04.24.
//

#ifndef SUNGEARENGINE_AUDIOPROCESSOR_H
#define SUNGEARENGINE_AUDIOPROCESSOR_H

#include <SGCore/Scene/ISystem.h>

namespace SGCore
{
    /**
     * System to update all AudioSource components of all entities on scene.\n
     * Updates position, direction and state of audio sources.\n
     * Uses SGCore::Transform to update position and direction.\n
     * Invokes SGCore::AudioSource::onStateChanged signal if audio source state has changed.
     */
    struct AudioProcessor : public ISystem
    {
        sg_implement_type_id(AudioProcessor, 27)

        void update(const double& dt, const double& fixedDt) override;
    };
}

#endif // SUNGEARENGINE_AUDIOPROCESSOR_H
