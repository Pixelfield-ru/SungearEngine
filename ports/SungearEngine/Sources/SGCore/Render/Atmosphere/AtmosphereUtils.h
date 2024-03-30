//
// Created by ilya on 15.02.24.
//

#ifndef SUNGEARENGINE_ATMOSPHEREUTILS_H
#define SUNGEARENGINE_ATMOSPHEREUTILS_H

#include "SGUtils/Math/MathUtils.h"
#include "Atmosphere.h"

namespace SGCore
{
    struct AtmosphereUtils
    {
        static glm::vec3 rayleighMieAtmosphere(glm::vec3 r, const glm::vec3& ro,
                                                glm::vec3 pSun, float iSun,
                                                float rPlanet, float rAtmos, const glm::vec3& kRlh, float kMie,
                                                float shRlh,
                                                float shMie, float g, int iSteps, int jSteps) noexcept;

        static glm::vec3 calculateSunColor(const Atmosphere& atmosphere) noexcept;
    };
}

#endif //SUNGEARENGINE_ATMOSPHEREUTILS_H
