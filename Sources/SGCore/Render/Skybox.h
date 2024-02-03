//
// Created by stuka on 02.02.2024.
//

#ifndef SUNGEARENGINE_SKYBOX_H
#define SUNGEARENGINE_SKYBOX_H

#include "MeshBase.h"

namespace SGCore
{
    struct Skybox
    {
        Skybox() noexcept;

        MeshBase m_base;
    };
}

#endif //SUNGEARENGINE_SKYBOX_H
