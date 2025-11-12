//
// Created by stuka on 07.01.2025.
//

#include "MotionPlanner.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IRenderer.h"

SGCore::MotionPlanner::MotionPlanner() noexcept
{
    m_bonesMatricesData.resize(m_maxBonesPerMesh * 16 + 4);

    m_bonesMatricesBuffer = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    m_bonesMatricesBuffer->m_textureBufferUsage = SGGUsage::SGG_DYNAMIC;
    m_bonesMatricesBuffer->m_type = SGTextureType::SG_TEXTURE_BUFFER;
    m_bonesMatricesBuffer->create<float>(m_bonesMatricesData.data(), std::uint32_t(m_bonesMatricesData.size() / 4) + 1 , 1, 1,
                                         SGGColorInternalFormat::SGG_RGBA32_FLOAT,
                                         SGGColorFormat::SGG_RGBA);
}

SGCore::MotionPlanner SGCore::MotionPlanner::copyStructure() const noexcept
{
    MotionPlanner newMotionPlanner;
    newMotionPlanner.m_skeleton = m_skeleton;
    newMotionPlanner.m_maxBonesPerMesh = m_maxBonesPerMesh;

    for(const auto& rootNode : m_rootNodes)
    {
        newMotionPlanner.m_rootNodes.push_back(rootNode->copyStructure());
    }

    return newMotionPlanner;
}

void SGCore::MotionPlanner::onMemberAssetsReferencesResolveImpl(SGCore::AssetManager* updatedAssetManager) noexcept
{
    AssetManager::resolveAssetReference(updatedAssetManager, m_skeleton);
}

