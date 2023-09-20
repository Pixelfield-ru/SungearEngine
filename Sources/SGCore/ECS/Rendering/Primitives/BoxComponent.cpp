#include "BoxComponent.h"

#include "SGCore/Graphics/GraphicsFilesResourcesManager.h"
#include "SGCore/Main/CoreMain.h"

Core::ECS::BoxComponent::BoxComponent()
{
    m_mesh = std::shared_ptr<ImportedScene::IMesh>(Core::Main::CoreMain::getRenderer().createMesh());

    // building a cube ------------------------------

    m_mesh->m_positions.push_back(-m_size.x / 2.0f);
    m_mesh->m_positions.push_back(-m_size.y / 2.0f);
    m_mesh->m_positions.push_back(m_size.z / 2.0f);

    m_mesh->m_positions.push_back(-m_size.x / 2.0f);
    m_mesh->m_positions.push_back(-m_size.y / 2.0f);
    m_mesh->m_positions.push_back(-m_size.z / 2.0f);

    m_mesh->m_positions.push_back(-m_size.x / 2.0f);
    m_mesh->m_positions.push_back(m_size.y / 2.0f);
    m_mesh->m_positions.push_back(-m_size.z / 2.0f);

    m_mesh->m_positions.push_back(-m_size.x / 2.0f);
    m_mesh->m_positions.push_back(m_size.y / 2.0f);
    m_mesh->m_positions.push_back(m_size.z / 2.0f);



    m_mesh->m_positions.push_back(m_size.x / 2.0f);
    m_mesh->m_positions.push_back(-m_size.y / 2.0f);
    m_mesh->m_positions.push_back(m_size.z / 2.0f);

    m_mesh->m_positions.push_back(m_size.x / 2.0f);
    m_mesh->m_positions.push_back(-m_size.y / 2.0f);
    m_mesh->m_positions.push_back(-m_size.z / 2.0f);

    m_mesh->m_positions.push_back(m_size.x / 2.0f);
    m_mesh->m_positions.push_back(m_size.y / 2.0f);
    m_mesh->m_positions.push_back(-m_size.z / 2.0f);

    m_mesh->m_positions.push_back(m_size.x / 2.0f);
    m_mesh->m_positions.push_back(m_size.y / 2.0f);
    m_mesh->m_positions.push_back(m_size.z / 2.0f);

    // -------------------

    m_mesh->m_indices.push_back(0);
    m_mesh->m_indices.push_back(1);

    m_mesh->m_indices.push_back(1);
    m_mesh->m_indices.push_back(2);

    m_mesh->m_indices.push_back(2);
    m_mesh->m_indices.push_back(3);

    m_mesh->m_indices.push_back(3);
    m_mesh->m_indices.push_back(0);



    m_mesh->m_indices.push_back(4);
    m_mesh->m_indices.push_back(5);

    m_mesh->m_indices.push_back(5);
    m_mesh->m_indices.push_back(6);

    m_mesh->m_indices.push_back(6);
    m_mesh->m_indices.push_back(7);

    m_mesh->m_indices.push_back(7);
    m_mesh->m_indices.push_back(4);



    m_mesh->m_indices.push_back(0);
    m_mesh->m_indices.push_back(4);

    m_mesh->m_indices.push_back(3);
    m_mesh->m_indices.push_back(7);

    m_mesh->m_indices.push_back(2);
    m_mesh->m_indices.push_back(6);

    m_mesh->m_indices.push_back(1);
    m_mesh->m_indices.push_back(5);


    m_mesh->m_material->setShader(
            std::shared_ptr<Graphics::IShader>(
                    Core::Main::CoreMain::getRenderer().createShader(
                            Graphics::getShaderPath(Graphics::StandardShaderType::SG_COMPLEX_PRIMITIVES_SHADER)
                    )
            )
    );

    m_mesh->prepare();
}

void Core::ECS::BoxComponent::setVertexPosition(const size_t& vertexIdx, const float& x, const float& y,
                                                const float& z) noexcept
{

}