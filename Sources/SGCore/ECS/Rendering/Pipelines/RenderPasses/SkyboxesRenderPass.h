//
// Created by Ilya on 25.11.2023.
//

#ifndef SUNGEARENGINE_SKYBOXESRENDERPASS_H
#define SUNGEARENGINE_SKYBOXESRENDERPASS_H

#include "IRenderPass.h"

namespace SGCore
{
    struct SkyboxesRenderPass : public IRenderPass
    {
        void render() final;
    };
}

#endif //SUNGEARENGINE_SKYBOXESRENDERPASS_H
