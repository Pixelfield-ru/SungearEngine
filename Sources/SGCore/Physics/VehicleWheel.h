//
// Created by stuka on 24.07.2026.
//

#pragma once

#include <glm/vec3.hpp>
#include <BulletDynamics/Vehicle/btWheelInfo.h>

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct WheeledVehicle;

    struct VehicleWheel final : ECS::Component<VehicleWheel, const VehicleWheel>
    {
        struct CreationParams
        {
            glm::vec3 m_direction { 0, -1, 0 };
            glm::vec3 m_axle { 0, 0, 1 };
            float m_suspensionRestLength = 1.0f;
            float m_wheelRadius = 1.0f;
            bool m_isFront {};
        };

        ~VehicleWheel() noexcept override;

        btWheelInfo* m_wheelInfo {};
        WheeledVehicle* m_parentVehicle {};
        std::int32_t m_wheelIndex = -1;

        float m_steering {};
        float m_engineForce {};
        float m_brake {};
    };
}