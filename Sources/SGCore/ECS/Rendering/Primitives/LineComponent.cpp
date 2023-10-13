#include "LineComponent.h"

#include "SGCore/Memory/Assets/ShaderAsset.h"
#include "SGCore/Graphics/Defines.h"
#include "SGCore/Main/CoreMain.h"

Core::ECS::LineComponent::LineComponent() noexcept
{
    m_mesh = std::shared_ptr<ImportedScene::IMesh>(Core::Main::CoreMain::getRenderer().createMesh());

    m_mesh->m_useIndices = false;

    // first point
    m_mesh->m_positions.push_back(0.0);
    m_mesh->m_positions.push_back(0.0);
    m_mesh->m_positions.push_back(0.0);

    // second point
    m_mesh->m_positions.push_back(0.0);
    m_mesh->m_positions.push_back(10.0);
    m_mesh->m_positions.push_back(0.0);

    m_mesh->m_material->setShader(
            SGMAT_STANDARD_SHADER_NAME,
            std::shared_ptr<Graphics::IShader>(
                    Core::Main::CoreMain::getRenderer().createShader(
                            Graphics::getShaderPath(Graphics::StandardShaderType::SG_LINES_SHADER)
                    )
            )
    );

    //m_mesh->m_material->setCurrentShader(SGMAT_STANDARD_SHADER_NAME);

    const auto& materialShader = m_mesh->m_material->getCurrentShader();

    materialShader->bind();

    materialShader->useVectorf(
            "verticesPositions[" + std::to_string(0) + "]",
            0.0, 0.0, 0.0
    );

    materialShader->useVectorf(
            "verticesPositions[" + std::to_string(1) + "]",
            0.0, 10.0, 0.0
    );
}

void Core::ECS::LineComponent::setVertexPosition
(const size_t& vertexIdx, const float& x, const float& y, const float& z) noexcept
{
    if(vertexIdx > 1) return;

    float curX;
    float curY;
    float curZ;

    m_mesh->getVertexPosition(vertexIdx, curX, curY, curZ);

    if(x != curX || y != curY || z != curZ)
    {
        const auto& materialShader = m_mesh->m_material->getCurrentShader();

        if(!materialShader) return;

        materialShader->bind();

        materialShader->useVectorf(
                "verticesPositions[" + std::to_string(vertexIdx) + "]",
                x, y, z);
    }
}
