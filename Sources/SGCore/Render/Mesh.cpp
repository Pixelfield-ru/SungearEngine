//
// Created by stuka on 02.01.2024.
//
#include "Mesh.h"
#include "SGCore/Graphics/API/IShader.h"

SGCore::Mesh::Mesh() noexcept
{
    onMeshCreated("StandardMeshShader");
}
