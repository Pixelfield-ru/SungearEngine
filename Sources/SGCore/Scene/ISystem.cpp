//
// Created by stuka on 30.10.2023.
//

#include "ISystem.h"
#include "SGCore/Scene/Scene.h"

void SGCore::ISystem::setScene(const SGCore::Ref<SGCore::Scene>& scene) noexcept
{
    m_scene = scene;
    onAddToScene(scene);
}

SGCore::Ref<SGCore::Scene> SGCore::ISystem::getScene() const noexcept
{
    return m_scene.lock();
}

std::shared_ptr<SGCore::Threading::Thread> SGCore::ISystem::getThread() const noexcept
{
    return m_thread;
}
