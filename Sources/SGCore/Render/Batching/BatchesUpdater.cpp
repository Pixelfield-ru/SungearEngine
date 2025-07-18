//
// Created by stuka on 18.07.2025.
//

#include "BatchesUpdater.h"

#include "Batch.h"
#include "SGCore/Scene/Scene.h"

void SGCore::BatchesUpdater::update(const double& dt, const double& fixedDt) noexcept
{
    const auto scene = getScene();

    if(!scene) return;

    const auto registry = scene->getECSRegistry();

    const auto batchesView = registry->view<Batch>();

    batchesView.each([&registry](Batch::reg_t& batch) {
        batch.update(*registry);
    });
}
