#pragma once

#ifndef SUNGEARENGINE_GL3MESH_H
#define SUNGEARENGINE_GL3MESH_H

#include "SGCore/ImportedScenesArch/IMesh.h"

namespace Core::Graphics::GL
{
    class GL3Mesh : public ImportedScene::IMesh
    {
    public:
        /**
         * Prepares a mesh. Creates VAO, VBO (for UV, vertex positions, normals), IBO.
         */
        void prepare() final;
    };
}

#endif //SUNGEARENGINE_GL3MESH_H
