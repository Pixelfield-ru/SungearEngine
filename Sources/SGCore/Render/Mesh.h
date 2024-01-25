#pragma once

#ifndef SUNGEARENGINE_MESHCOMPONENT_H
#define SUNGEARENGINE_MESHCOMPONENT_H

#include "SGCore/ECS/IComponent.h"
#include "MeshBase.h"

namespace SGCore
{
    struct Mesh : public MeshBase
    {
        Mesh() noexcept;
    };
}

#endif //SUNGEARENGINE_MESHCOMPONENT_H
