//
// Created by ilya on 15.02.24.
//

#pragma once

#include "SGCore/Math/MathUtils.h"
#include "Atmosphere.h"

namespace SGCore
{
    struct SGCORE_EXPORT AtmosphereUtils
    {
        static glm::vec3 rayleighMieAtmosphere(glm::vec3 r, const glm::vec3& ro,
                                                glm::vec3 pSun, float iSun,
                                                float rPlanet, float rAtmos, const glm::vec3& kRlh, float kMie,
                                                float shRlh,
                                                float shMie, float g, int iSteps, int jSteps) noexcept;

        static glm::vec3 calculateSunColor(const Atmosphere& atmosphere) noexcept;
    };
}
