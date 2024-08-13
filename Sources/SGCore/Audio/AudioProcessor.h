//
// Created by ilya on 15.04.24.
//

#ifndef SUNGEARENGINE_AUDIOPROCESSOR_H
#define SUNGEARENGINE_AUDIOPROCESSOR_H

#include <SGCore/Scene/ISystem.h>

namespace SGCore
{
    struct AudioProcessor : public ISystem
    {
        void update(const double& dt, const double& fixedDt) override;
    };
}

#endif // SUNGEARENGINE_AUDIOPROCESSOR_H
