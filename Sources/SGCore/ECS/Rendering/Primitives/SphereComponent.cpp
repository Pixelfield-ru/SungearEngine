#include "SphereComponent.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/GraphicsFilesResourcesManager.h"

Core::ECS::SphereComponent::SphereComponent()
{
    m_mesh = std::shared_ptr<ImportedScene::IMesh>(Core::Main::CoreMain::getRenderer().createMesh());

    m_mesh->m_material->setShader(
            std::shared_ptr<Graphics::IShader>(
                    Core::Main::CoreMain::getRenderer().createShader(
                            Graphics::getShaderPath(Graphics::StandardShaderType::SG_COMPLEX_PRIMITIVES_SHADER)
                    )
            )
    );
}

void Core::ECS::SphereComponent::setVertexPosition(const size_t& vertexIdx, const float& x, const float& y,
                                                   const float& z) noexcept
{

}
