//
// Created by stuka on 02.02.2024.
//

#ifndef SUNGEARENGINE_ATMOSPHERESCATTERING_H
#define SUNGEARENGINE_ATMOSPHERESCATTERING_H

#include <glm/vec3.hpp>
#include <map>

#include "SGCore/Render/MeshBase.h"

namespace SGCore
{
    struct AtmosphereScattering
    {
        glm::vec3 m_sunPosition = { 0.0, 0.0, 0.0 };
        glm::vec3 m_sunRotation = { 0.0, 0.0, 0.0 };
        glm::vec3 m_lastSunRotation = { -1.0, 0.0, 0.0 };
        
        glm::vec4 m_sunColor = { 1.0, 1.0, 1.0, 1.0 };
        glm::vec4 m_lastSunColor = { 0.0, 0.0, 0.0, 1.0 };
        
        // first - hashed sun position
        // second - calculated sun color
        std::map<size_t, glm::vec3> m_precalculatedSunColors;
        
        static auto getSunOrigin() noexcept
        {
            return s_sunOrigin;
        }
        
    private:
        static inline glm::vec3 s_sunOrigin = { 0.0, 1.0, 0.0 };
    };
}

#endif //SUNGEARENGINE_ATMOSPHERESCATTERING_H
