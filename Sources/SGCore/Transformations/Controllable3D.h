//
// Created by stuka on 03.02.2024.
//

#ifndef SUNGEARENGINE_CONTROLLABLE3D_H
#define SUNGEARENGINE_CONTROLLABLE3D_H

#include "SGCore/Annotations/Annotations.h"

namespace SGCore
{
    sg_struct()
    struct Controllable3D
    {
        sg_member()
        float m_movementSpeed = 0.075f;
        sg_member()
        float m_rotationSensitive = 0.2f;
    };
}

#endif //SUNGEARENGINE_CONTROLLABLE3D_H
