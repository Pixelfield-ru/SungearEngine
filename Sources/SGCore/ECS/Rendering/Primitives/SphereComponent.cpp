#include "SphereComponent.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/Defines.h"

Core::ECS::SphereComponent::SphereComponent()
{
    m_mesh = std::shared_ptr<ImportedScene::IMesh>(Core::Main::CoreMain::getRenderer().createMesh());

    /*m_mesh->m_material = std::shared_ptr<Memory::Assets::IMaterial>(
            Core::Main::CoreMain::getRenderer().createBlankStandardMaterial(
                    Graphics::StandardShaderType::SG_COMPLEX_PRIMITIVES_SHADER
            )
    );*/
}

void Core::ECS::SphereComponent::setVertexPosition(const size_t& vertexIdx, const float& x, const float& y,
                                                   const float& z) noexcept
{

}
