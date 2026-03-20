#pragma once

#include "MeshBase.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct SGCORE_EXPORT Mesh : ECS::Component<Mesh, const Mesh>
    {
        MeshBase m_base;

        Mesh() noexcept;
    };
}
