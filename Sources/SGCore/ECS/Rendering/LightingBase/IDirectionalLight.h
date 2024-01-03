//
// Created by stuka on 02.01.2024.
//

#ifndef SUNGEARENGINE_IDIRECTIONALLIGHT_H
#define SUNGEARENGINE_IDIRECTIONALLIGHT_H

#include "ILight.h"

namespace SGCore
{
    struct IDirectionalLight : public ILight
    {
        IDirectionalLight() noexcept;

    private:
        void init() noexcept final { }
    };
}

#endif //SUNGEARENGINE_IDIRECTIONALLIGHT_H
