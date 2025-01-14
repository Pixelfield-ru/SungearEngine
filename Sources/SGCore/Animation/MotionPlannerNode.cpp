//
// Created by stuka on 07.01.2025.
//
#include "MotionPlannerNode.h"
#include "MotionPlannerConnection.h"

SGCore::Ref<SGCore::MotionPlannerNode> SGCore::MotionPlannerNode::copyStructure() const noexcept
{
    auto newNode = createNode();
    newNode->m_skeletalAnimation = m_skeletalAnimation;
    newNode->m_animationSpeed = m_animationSpeed;
    newNode->m_isRepeated = m_isRepeated;
    newNode->m_activationAction = m_activationAction->copy();

    for(const auto& connection : m_connections)
    {
        newNode->m_connections.push_back(MakeRef<MotionPlannerConnection>(connection->copyStructure(newNode)));
    }

    return newNode;
}

SGCore::Ref<SGCore::MotionPlannerNode> SGCore::MotionPlannerNode::createNode() noexcept
{
    return SGCore::Ref<SGCore::MotionPlannerNode>(new MotionPlannerNode);
}

void SGCore::MotionPlannerNode::onMemberAssetsReferencesResolveImpl(SGCore::AssetManager* updatedAssetManager) noexcept
{
    AssetManager::resolveAssetReference(updatedAssetManager, m_skeletalAnimation);
}

