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

double SGCore::ISystem::getUpdateFunctionExecutionTime() const noexcept
{
    return m_update_executionTime;
}

double SGCore::ISystem::getFixedUpdateFunctionExecutionTime() const noexcept
{
    return m_fixedUpdate_executionTime;
}
