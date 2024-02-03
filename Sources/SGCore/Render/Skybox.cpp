//
// Created by stuka on 02.02.2024.
//

#include "Skybox.h"
#include "MeshesUtils.h"

SGCore::Skybox::Skybox() noexcept
{
    MeshesUtils::loadMeshShader(m_base, "SkyboxShader");
}
