#include "SphereGizmo.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/Defines.h"

Core::ECS::SphereGizmo::SphereGizmo()
{
    m_meshData = std::shared_ptr<ImportedScene::IMeshData>(Core::Main::CoreMain::getRenderer().createMeshData());

    /*m_mesh->m_material = std::shared_ptr<Memory::Assets::IMaterial>(
            Core::Main::CoreMain::getRenderer().createBlankStandardMaterial(
                    Graphics::StandardShaderType::SG_COMPLEX_PRIMITIVES_SHADER
            )
    );*/
}

void Core::ECS::SphereGizmo::setVertexPosition(const size_t& vertexIdx, const float& x, const float& y,
                                               const float& z) noexcept
{

}
