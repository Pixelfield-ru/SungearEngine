#pragma once

#ifndef SUNGEARENGINE_GL3MESH_H
#define SUNGEARENGINE_GL3MESH_H

#include "SGCore/Graphics/IMesh.h"

namespace Core::Graphics::API::GL::GL3
{
    class GL3Mesh : public IMesh
    {
    public:
        void prepare() final;
    };
}

#endif //SUNGEARENGINE_GL3MESH_H
