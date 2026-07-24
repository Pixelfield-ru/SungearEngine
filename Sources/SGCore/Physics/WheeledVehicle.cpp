//
// Created by stuka on 22.07.2026.
//

#include "WheeledVehicle.h"

#include "PhysicsWorld3D.h"
#include "Rigidbody3D.h"
#include "VehicleWheel.h"

SGCore::WheeledVehicle::WheeledVehicle(Ref<PhysicsWorld3D> parentPhysicsWorld,
                                       const Rigidbody3D& chassisBody,
                                       const btRaycastVehicle::btVehicleTuning& tuning) noexcept
{
    m_parentPhysicsWorld = parentPhysicsWorld;

    m_tuning = tuning;

    m_raycaster = MakeScope<btDefaultVehicleRaycaster>(parentPhysicsWorld->getDynamicsWorld().get());
    m_vehicle = MakeScope<btRaycastVehicle>(m_tuning, chassisBody.m_body.get(), m_raycaster.get());

    m_vehicle->setCoordinateSystem(0, 1, 2);
    parentPhysicsWorld->addVehicle(m_vehicle.get());
}

SGCore::WheeledVehicle::~WheeledVehicle()
{
    auto lockedWorld = m_parentPhysicsWorld.lock();
    if(!lockedWorld) return;

    const auto ecsRegistry = lockedWorld->getScene()->getECSRegistry();

    if(m_vehicle)
    {
        for(auto wheelEntity : m_wheels)
        {
            ecsRegistry->erase<VehicleWheel>(wheelEntity);
        }

        lockedWorld->removeVehicle(m_vehicle.get());
    }
}

void SGCore::WheeledVehicle::addWheel(const VehicleWheel::CreationParams& params, ECS::entity_t wheelEntity) noexcept
{
    auto lockedWorld = m_parentPhysicsWorld.lock();
    if(!lockedWorld)
    {
        return;
    }

    const auto ecsRegistry = lockedWorld->getScene()->getECSRegistry();

    const auto* wheelTransform = ecsRegistry->tryGet<Transform>(wheelEntity);

    if(!wheelTransform || ecsRegistry->allOf<VehicleWheel>(wheelEntity)) return;

    auto& wheel = ecsRegistry->emplace<VehicleWheel>(wheelEntity);

    auto& localTransform = wheelTransform->m_localTransform;
    auto& localPosition = localTransform.m_position;

    auto& wheelInfo = m_vehicle->addWheel(
        btVector3(localPosition.x, localPosition.y, localPosition.z),
        btVector3(params.m_direction.x, params.m_direction.y, params.m_direction.z),
        btVector3(params.m_axle.x, params.m_axle.y, params.m_axle.z),
        params.m_suspensionRestLength,
        params.m_wheelRadius,
        m_tuning,
        params.m_isFront
    );

    wheel.m_wheelInfo = &wheelInfo;
    wheel.m_parentVehicle = this;
    wheel.m_wheelIndex = m_vehicle->getNumWheels() - 1;
}

void SGCore::WheeledVehicle::removeWheel(ECS::entity_t wheelEntity) noexcept
{
    removeWheelImpl(wheelEntity, true);
}

const std::vector<SGCore::ECS::entity_t>& SGCore::WheeledVehicle::getWheels() const noexcept
{
    return m_wheels;
}

void SGCore::WheeledVehicle::removeWheelImpl(ECS::entity_t wheelEntity, bool eraseComponent) noexcept
{
    auto lockedWorld = m_parentPhysicsWorld.lock();
    if(!lockedWorld)
    {
        return;
    }

    const auto ecsRegistry = lockedWorld->getScene()->getECSRegistry();

    const auto* wheel = ecsRegistry->tryGet<VehicleWheel>(wheelEntity);

    if(!wheel || wheel->m_wheelIndex >= m_vehicle->getNumWheels()) return;

    // swap index
    auto& lastWheel = ecsRegistry->get<VehicleWheel>(m_wheels[m_vehicle->getNumWheels() - 1]);
    lastWheel.m_wheelIndex = wheel->m_wheelIndex;

    m_vehicle->m_wheelInfo.removeAtIndex(wheel->m_wheelIndex);
    m_wheels.erase(m_wheels.begin() + wheel->m_wheelIndex);

    if(eraseComponent) ecsRegistry->erase<VehicleWheel>(wheelEntity);
}
