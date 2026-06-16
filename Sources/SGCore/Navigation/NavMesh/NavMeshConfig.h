//
// Created by stuka on 06.12.2025.
//

#pragma once

#include <cstdint>
#include <sgcore_export.h>

namespace SGCore::Navigation
{
    struct SGCORE_EXPORT NavMeshConfig
    {
        float m_agentRadius = 0.5f;
        float m_agentHeight = 2.0f;
        float m_agentMaxClimb = 0.5f;
        float m_agentMaxSlope = 45.0f;
        float m_cellSize = 5.0f;
        float m_cellHeight = 5.0f;
        float m_maxSimplificationError = 1.3f;
        float m_edgeMaxLength = 20.0f;
        std::int32_t m_minRegionSize = 8;
        std::int32_t m_mergeRegionSize = 20;

        std::int32_t m_maxVerticesPerPoly = 100;

        float m_detailSampleDistance = 10.0f;
        float m_detailSampleMaxError = 2.0f;
    };
}