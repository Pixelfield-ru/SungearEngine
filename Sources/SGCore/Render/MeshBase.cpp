//
// Created by stuka on 19.01.2024.
//

#include "MeshBase.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "RenderPipelinesManager.h"

void SGCore::MeshBase::setMeshData(const SGCore::AssetRef<SGCore::IMeshData>& meshData) noexcept
{
    m_meshData = meshData;
    if(!m_material && meshData)
    {
        m_material = meshData->m_material;
    }
}

SGCore::AssetRef<SGCore::IMeshData> SGCore::MeshBase::getMeshData() const noexcept
{
    return m_meshData;
}

void SGCore::MeshBase::setMaterial(const SGCore::AssetRef<SGCore::IMaterial>& material) noexcept
{
    m_material = material;
}

SGCore::AssetRef<SGCore::IMaterial> SGCore::MeshBase::getMaterial() const noexcept
{
    return m_material;
}

void SGCore::MeshBase::onMemberAssetsReferencesResolve(SGCore::AssetManager* updatedAssetManager) noexcept
{
    AssetManager::resolveAssetReference(updatedAssetManager, m_meshData);
    AssetManager::resolveAssetReference(updatedAssetManager, m_material);
}


