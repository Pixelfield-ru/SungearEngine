//
// Created by stuka on 06.12.2025.
//

#pragma once

#include <cstdint>

namespace SGCore::Navigation
{
    struct NavMeshConfig
    {
        float m_agentRadius = 0.5f;
        float m_agentHeight = 2.0f;
        float m_agentMaxClimb = 0.5f;
        float m_agentMaxSlope = 45.0f;
        float m_cellSize = 0.3f;
        float m_cellHeight = 0.2f;
        float m_maxSimplificationError = 1.3f;
        std::int32_t m_minRegionArea = 8;
        std::int32_t m_mergeRegionArea = 20;
    };
}