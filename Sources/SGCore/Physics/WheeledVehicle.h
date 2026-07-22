//
// Created by stuka on 22.07.2026.
//

#pragma once

#include <BulletDynamics/Vehicle/btRaycastVehicle.h>

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct Rigidbody3D;
    struct PhysicsWorld3D;

    struct SGCORE_EXPORT WheeledVehicle final : ECS::Component<WheeledVehicle, const WheeledVehicle>
    {
        Scope<btRaycastVehicle> m_vehicle {};
        Scope<btVehicleRaycaster> m_raycaster {};

        Weak<PhysicsWorld3D> m_parentPhysicsWorld;

        ~WheeledVehicle() override;
    };

    void configureVehicle(WheeledVehicle& vehicle, Rigidbody3D& vehicleBody);
}
