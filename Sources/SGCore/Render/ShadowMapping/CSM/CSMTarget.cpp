//
// Created by stuka on 08.07.2025.
//

#include "CSMTarget.h"


SGCore::CSMTarget::CSMTarget()
{
    CSMCascade cascade0;
    cascade0.m_level = 50.0f;
    cascade0.m_frameBufferSize = { 4096, 4096 };
    cascade0.createFrameBuffer();
    m_levels.push_back(cascade0);

    CSMCascade cascade1;
    cascade1.m_level = 25.0f;
    cascade1.m_frameBufferSize = { 2048, 2048 };
    cascade1.createFrameBuffer();
    m_levels.push_back(cascade1);

    CSMCascade cascade2;
    cascade2.m_level = 10.0f;
    cascade2.m_frameBufferSize = { 1024, 1024 };
    cascade2.createFrameBuffer();
    m_levels.push_back(cascade2);

    CSMCascade cascade3;
    cascade3.m_level = 2.0f;
    cascade3.m_frameBufferSize = { 512, 512 };
    cascade3.createFrameBuffer();
    m_levels.push_back(cascade3);
}

const std::vector<SGCore::CSMCascade>& SGCore::CSMTarget::getCascades() const noexcept
{
    return m_levels;
}
