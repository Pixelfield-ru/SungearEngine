//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_DIRECTIONALLIGHTCOMPONENT_H
#define SUNGEARENGINE_DIRECTIONALLIGHTCOMPONENT_H

#include "ILight.h"

namespace SGCore
{
    class DirectionalLight : public ILight
    {
    private:
        void init() noexcept final { }
    };
}

#endif //SUNGEARENGINE_DIRECTIONALLIGHTCOMPONENT_H
