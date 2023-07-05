//
// Created by stuka on 04.07.2023.
//

#include "Scene.h"

std::shared_ptr<Core::ECS::Scene> Core::ECS::Scene::getCurrentScene() noexcept
{
    return m_currentScene;
}

void Core::ECS::Scene::setCurrentScene(const std::shared_ptr<Scene>& newCurrentScene) noexcept
{
    m_currentScene = newCurrentScene;
}
