//
// Created by stuka on 08.07.2025.
//

#include "CSMTarget.h"


SGCore::CSMTarget::CSMTarget()
{
    CSMCascade cascade0;
    cascade0.m_level = 250.0f;
    cascade0.m_frameBufferSize = { 1024 * 2, 1024 * 2 };
    cascade0.m_bias = 0.0012f;
    cascade0.createFrameBuffer();
    m_levels.push_back(cascade0);

    CSMCascade cascade1;
    cascade1.m_level = 150.0f;
    cascade1.m_frameBufferSize = { 1024 * 2, 1024 * 2 };
    cascade1.m_bias = 0.002f;
    cascade1.createFrameBuffer();
    m_levels.push_back(cascade1);

    CSMCascade cascade2;
    cascade2.m_level = 50.0f;
    cascade2.m_frameBufferSize = { 1024 * 2, 1024 * 2 };
    cascade2.m_bias = 0.003f;
    cascade2.createFrameBuffer();
    m_levels.push_back(cascade2);

    CSMCascade cascade3;
    cascade3.m_level = 10.0f;
    cascade3.m_frameBufferSize = { 1024 * 2, 1024 * 2 };
    cascade3.m_bias = 0.008f;
    cascade3.createFrameBuffer();
    m_levels.push_back(cascade3);
}

const std::vector<SGCore::CSMCascade>& SGCore::CSMTarget::getCascades() const noexcept
{
    return m_levels;
}

SGCore::CSMCascade& SGCore::CSMTarget::getCascade(std::uint8_t idx) noexcept
{
    return m_levels[idx];
}

const SGCore::CSMCascade& SGCore::CSMTarget::getCascade(std::uint8_t idx) const noexcept
{
    return m_levels[idx];
}
