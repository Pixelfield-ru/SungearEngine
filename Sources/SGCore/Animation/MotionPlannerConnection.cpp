//
// Created by stuka on 07.01.2025.
//
#include "MotionPlannerConnection.h"
#include "MotionPlannerNode.h"

SGCore::MotionPlannerConnection SGCore::MotionPlannerConnection::copyStructure(const MotionPlannerNode& baseNode) const noexcept
{
    MotionPlannerConnection newConnection;

    newConnection.m_blendTime = m_blendTime;
    newConnection.m_blendSpeed = m_blendSpeed;

    newConnection.m_previousNode = MakeRef<MotionPlannerNode>(baseNode);
    newConnection.m_nextNode = MakeRef<MotionPlannerNode>(m_nextNode->copyStructure());

    return newConnection;
    // newConnection->
}
