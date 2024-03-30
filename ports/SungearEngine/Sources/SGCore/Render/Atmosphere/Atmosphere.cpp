//
// Created by Ilya on 15.02.2024.
//

#include "Atmosphere.h"

SGCore::Atmosphere::Atmosphere() noexcept
{
    m_rayleighScatteringCoeff = { 14.5e-6, 15.0e-6, 25.4e-6 };

    m_mieScatteringCoeff = 5e-6;

    m_rayleighScaleHeight = 8e3;

    m_mieScaleHeight = 1.0e3;

    m_sunIntensity = 30.0f;

    m_planetRadius = 6371e3;

    m_atmosphereRadius = 6471e3;

    m_miePreferredScatteringDirection = 0.958;

    m_rayOrigin = { 0, 6372e3, 0 };
}
