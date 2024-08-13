//
// Created by stuka on 02.02.2024.
//

#ifndef SUNGEARENGINE_ATMOSPHERE_H
#define SUNGEARENGINE_ATMOSPHERE_H

#include <SGCore/pch.h>

#include "SGCore/Render/MeshBase.h"
#include "SGCore/Annotations/Annotations.h"

namespace SGCore
{
    sg_struct()
    struct Atmosphere
    {
        Atmosphere() noexcept;
        Atmosphere(const Atmosphere&) noexcept = default;
        Atmosphere(Atmosphere&& atmosphere) noexcept = default;

        sg_member()
        glm::vec3 m_sunPosition { 0.0, 0.0, 0.0 };
        sg_member()
        glm::vec3 m_sunAmbient { 0, 0, 0 };
        sg_member()
        glm::vec3 m_lastSunAmbient  { 0, 0, 0 };
        sg_member()
        glm::vec3 m_sunRotation { 0.0, 0.0, 0.0 };
        sg_member()
        glm::vec3 m_lastSunRotation { -1.0, 0.0, 0.0 };

        // ----

        sg_member()
        glm::vec3 m_rayleighScatteringCoeff { };
        sg_member()
        glm::vec3 m_lastRayleighScatteringCoeff { 0.0, 0.0, 0.0 };

        sg_member()
        float m_mieScatteringCoeff = 0.0f;
        sg_member()
        float m_lastMieScatteringCoeff = 0.0f;

        sg_member()
        float m_rayleighScaleHeight = 0.0f;
        sg_member()
        float m_lastRayleighScaleHeight = 0;

        sg_member()
        float m_mieScaleHeight = 0.0f;
        sg_member()
        float m_lastMieScaleHeight = 0.0f;

        sg_member()
        float m_sunIntensity = 0.0f;
        sg_member()
        float m_lastSunIntensity = 0.0f;

        // in meters
        sg_member()
        float m_planetRadius = 0.0f;
        sg_member()
        float m_lastPlanetRadius = 0.0f;

        // in meters
        sg_member()
        float m_atmosphereRadius = 0.0f;
        sg_member()
        float m_lastAtmosphereRadius = 0.0f;

        sg_member()
        float m_miePreferredScatteringDirection = 0.0f;
        sg_member()
        float m_lastMiePreferredScatteringDirection = 0;

        sg_member()
        glm::vec3 m_rayOrigin { };
        sg_member()
        glm::vec3 m_lastRayOrigin { 0, 0, 0 };

        // ----
        
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

#endif //SUNGEARENGINE_ATMOSPHERE_H
