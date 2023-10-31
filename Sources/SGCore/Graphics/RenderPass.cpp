//
// Created by Ilya on 30.10.2023.
//

#include "RenderPass.h"

#include "SGCore/Main/CoreMain.h"

Core::Graphics::RenderOutput::RenderOutput()
{
    m_billboard = std::shared_ptr<ImportedScene::IMesh>(Core::Main::CoreMain::getRenderer().createMesh());

    m_billboard->m_material = std::shared_ptr<Memory::Assets::IMaterial>(Core::Main::CoreMain::getRenderer().createPostProcessingMaterial());

    /*m_billboard->m_indices.push_back(0);
    m_billboard->m_indices.push_back(1);

    m_billboard->m_indices.push_back(1);
    m_billboard->m_indices.push_back(2);

    m_billboard->m_indices.push_back(2);
    m_billboard->m_indices.push_back(0);

    m_billboard->m_indices.push_back(0);
    m_billboard->m_indices.push_back(3);

    m_billboard->m_indices.push_back(3);
    m_billboard->m_indices.push_back(2);*/

    m_billboard->m_indices.push_back(0);
    m_billboard->m_indices.push_back(1);

    m_billboard->m_indices.push_back(2);
    m_billboard->m_indices.push_back(0);

    m_billboard->m_indices.push_back(3);
    m_billboard->m_indices.push_back(2);

    m_billboard->prepare();
}
