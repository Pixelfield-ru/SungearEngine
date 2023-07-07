//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKMESH_H
#define SUNGEARENGINE_VKMESH_H

#include "SGCore/ImportedScenesArch/IMesh.h"

namespace Core::Graphics
{
    // TODO: impl VkMesh
    class VkMesh : public ImportedScene::IMesh
    {
    public:
        void prepare() final;
    };
}

#endif //SUNGEARENGINE_VKMESH_H
