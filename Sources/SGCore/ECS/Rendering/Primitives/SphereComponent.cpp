#include "SphereComponent.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/Defines.h"

Core::ECS::SphereComponent::SphereComponent()
{
    m_mesh = std::shared_ptr<ImportedScene::IMesh>(Core::Main::CoreMain::getRenderer().createMesh());

    m_mesh->m_material->setShader(
            SGMAT_STANDARD_SHADER_NAME,
            std::shared_ptr<Graphics::IShader>(
                    Core::Main::CoreMain::getRenderer().createShader(
                            Graphics::getShaderPath(Graphics::StandardShaderType::SG_COMPLEX_PRIMITIVES_SHADER)
                    )
            )
    );

    //m_mesh->m_material->setCurrentShader(SGMAT_STANDARD_SHADER_NAME);
}

void Core::ECS::SphereComponent::setVertexPosition(const size_t& vertexIdx, const float& x, const float& y,
                                                   const float& z) noexcept
{

}
