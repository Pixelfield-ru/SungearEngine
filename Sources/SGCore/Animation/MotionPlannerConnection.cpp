//
// Created by stuka on 07.01.2025.
//
#include "MotionPlannerConnection.h"
#include "MotionPlannerNode.h"

SGCore::MotionPlannerConnection SGCore::MotionPlannerConnection::copyStructure(const Ref<MotionPlannerNode>& baseNode) const noexcept
{
    MotionPlannerConnection newConnection;

    newConnection.m_blendTime = m_blendTime;
    newConnection.m_blendSpeed = m_blendSpeed;

    newConnection.m_previousNode = baseNode;
    newConnection.m_nextNode = m_nextNode->copyStructure();

    return newConnection;
    // newConnection->
}
