//
// Created by stuka on 22.07.2026.
//

#pragma once

#include <vector>
#include <BulletDynamics/Vehicle/btRaycastVehicle.h>
#include <glm/vec3.hpp>

#include "VehicleWheel.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct Rigidbody3D;
    struct PhysicsWorld3D;

    struct SGCORE_EXPORT WheeledVehicle final : ECS::Component<WheeledVehicle, const WheeledVehicle>
    {
        friend struct VehicleWheel;

        btRaycastVehicle::btVehicleTuning m_tuning {};
        Scope<btRaycastVehicle> m_vehicle {};
        Scope<btVehicleRaycaster> m_raycaster {};

        Weak<PhysicsWorld3D> m_parentPhysicsWorld;

        WheeledVehicle(Ref<PhysicsWorld3D> parentPhysicsWorld,
                       const Rigidbody3D& chassisBody,
                       const btRaycastVehicle::btVehicleTuning& tuning) noexcept;

        ~WheeledVehicle() override;

        void addWheel(const VehicleWheel::CreationParams& params, ECS::entity_t wheelEntity) noexcept;
        void removeWheel(ECS::entity_t wheelEntity) noexcept;

        [[nodiscard]] const std::vector<ECS::entity_t>& getWheels() const noexcept;

    private:
        std::vector<ECS::entity_t> m_wheels;

        void removeWheelImpl(ECS::entity_t wheelEntity, bool eraseComponent) noexcept;
    };
}
