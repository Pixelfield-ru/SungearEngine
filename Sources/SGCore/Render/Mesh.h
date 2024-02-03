#pragma once

#ifndef SUNGEARENGINE_MESHCOMPONENT_H
#define SUNGEARENGINE_MESHCOMPONENT_H

#include "MeshBase.h"

namespace SGCore
{
    struct Mesh
    {
        MeshBase m_base;

        Mesh() noexcept;
    };
}

#endif //SUNGEARENGINE_MESHCOMPONENT_H
