#pragma once

#ifndef SUNGEARENGINE_GL3MESH_H
#define SUNGEARENGINE_GL3MESH_H

#include "SGCore/ImportedScenesArch/IMeshData.h"

namespace SGCore
{
    class GL3MeshData : public IMeshData
    {
    public:
        /**
         * Prepares a mesh. Creates VAO, VBO (for UV, vertex positions, normals), IBO.
         */
        void prepare() final;
    };
}

#endif //SUNGEARENGINE_GL3MESH_H
