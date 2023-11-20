#pragma once

#ifndef SUNGEARENGINE_MESHCOMPONENT_H
#define SUNGEARENGINE_MESHCOMPONENT_H

#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/ECS/IComponent.h"
#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"

namespace SGCore
{
    struct Mesh : public IComponent
    {
        MeshDataRenderInfo m_meshDataRenderInfo;
        Ref<IMeshData> m_meshData;

    protected:
        void init() noexcept final { }
    };
}

#endif //SUNGEARENGINE_MESHCOMPONENT_H
