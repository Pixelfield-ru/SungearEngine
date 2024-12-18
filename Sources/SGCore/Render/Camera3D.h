//
// Created by ilya on 25.02.24.
//

#ifndef SUNGEARENGINE_CAMERA3D_H
#define SUNGEARENGINE_CAMERA3D_H

#include <set>

#include "SGCore/Scene/EntityRef.h"

namespace SGCore
{
    struct Camera3D
    {
        // contains entities that can be picked for this camera
        std::unordered_set<EntityRef, EntityRefHash, EntityRefEqual> m_pickableEntities;

    private:
        volatile int m_dummy = 0;
    };
}

#endif //SUNGEARENGINE_CAMERA3D_H
