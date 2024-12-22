//
// Created by ilya on 25.02.24.
//

#ifndef SUNGEARENGINE_CAMERA3D_H
#define SUNGEARENGINE_CAMERA3D_H

#include <unordered_set>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct Camera3D
    {
        std::unordered_set<entity_t> m_pickedEntities;

    private:
        volatile int m_dummy = 0;
    };
}

#endif //SUNGEARENGINE_CAMERA3D_H
