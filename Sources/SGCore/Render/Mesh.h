#pragma once

#ifndef SUNGEARENGINE_MESHCOMPONENT_H
#define SUNGEARENGINE_MESHCOMPONENT_H

#include "MeshBase.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct Mesh : ECS::Component<Mesh, const Mesh>
    {
        MeshBase m_base;

        Mesh() noexcept;
    };
}

#endif //SUNGEARENGINE_MESHCOMPONENT_H
