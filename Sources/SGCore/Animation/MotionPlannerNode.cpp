//
// Created by stuka on 07.01.2025.
//
#include "MotionPlannerNode.h"
#include "MotionPlannerConnection.h"

SGCore::MotionPlannerNode SGCore::MotionPlannerNode::copyStructure() const noexcept
{
    MotionPlannerNode newNode;
    newNode.m_skeletalAnimation = m_skeletalAnimation;
    newNode.m_animationSpeed = m_animationSpeed;
    newNode.m_isRepeated = m_isRepeated;
    newNode.activationFunction = activationFunction;

    for(const auto& connection : m_connections)
    {
        newNode.m_connections.push_back(MakeRef<MotionPlannerConnection>(connection->copyStructure(newNode)));
    }

    return newNode;
}

