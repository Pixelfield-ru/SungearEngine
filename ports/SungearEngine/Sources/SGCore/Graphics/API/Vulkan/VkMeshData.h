//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKMESH_H
#define SUNGEARENGINE_VKMESH_H

#include "SGCore/ImportedScenesArch/IMeshData.h"

namespace SGCore
{
    // TODO: impl VkMesh
    class VkMeshData : public IMeshData
    {
    public:
        void prepare() final;
    };
}

#endif //SUNGEARENGINE_VKMESH_H
