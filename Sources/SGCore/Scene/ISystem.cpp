//
// Created by stuka on 30.10.2023.
//

#include "ISystem.h"
#include "SGCore/Scene/Scene.h"

void SGCore::ISystem::setScene(const SGCore::Ref<SGCore::Scene>& scene) noexcept
{
    m_scene = scene;
}

SGCore::Weak<SGCore::Scene> SGCore::ISystem::getScene() const noexcept
{
    return m_scene;
}

const size_t& SGCore::ISystem::getThreadID() const noexcept
{
    return m_threadID;
}
