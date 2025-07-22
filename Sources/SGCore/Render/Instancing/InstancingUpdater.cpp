//
// Created by stuka on 21.07.2025.
//

#include "InstancingUpdater.h"

#include "Instancing.h"
#include "SGCore/Scene/Scene.h"

void SGCore::InstancingUpdater::update(const double& dt, const double& fixedDt) noexcept
{
    const auto scene = getScene();

    if(!scene) return;

    const auto registry = scene->getECSRegistry();

    const auto instancingView = registry->view<Instancing>();

    instancingView.each([&registry](Instancing::reg_t& instancing) {
        instancing.update(*registry);
    });
}
