//
// Created by stuka on 24.07.2026.
//

#include "WheeledVehiclesUpdater.h"

#include "WheeledVehicle.h"
#include "SGCore/Scene/Scene.h"

void SGCore::WheeledVehiclesUpdater::update(double dt, double fixedDt) noexcept
{
    auto* scene = getScene();
    if(!scene) return;

    auto registry = scene->getECSRegistry();

    auto vehiclesView = registry->view<WheeledVehicle>();

    vehiclesView.each([&](WheeledVehicle& vehicle) {
        const auto& wheels = vehicle.getWheels();

        const auto& nativeVehicle = vehicle.m_vehicle;

        for(auto wheelEntity : wheels)
        {
            auto& wheel = registry->get<VehicleWheel>(wheelEntity);

            nativeVehicle->setSteeringValue(wheel.m_steering, wheel.m_wheelIndex);
            nativeVehicle->applyEngineForce(wheel.m_engineForce, wheel.m_wheelIndex);
            nativeVehicle->setBrake(wheel.m_brake, wheel.m_wheelIndex);

            nativeVehicle->updateWheelTransform(wheel.m_wheelIndex);
        }
    });
}
