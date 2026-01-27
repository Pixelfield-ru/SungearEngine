//
// Created by stuka on 26.01.2026.
//

#include "VolumetricFog.h"

#include "SGCore/Memory/Assets/ModelAsset.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"

SGCore::VolumetricFog::VolumetricFog()
{
    const auto mainAssetManager = AssetManager::getInstance();

    const auto standardCloudsMaterial = mainAssetManager->getOrAddAssetByAlias<IMaterial>("standard_volumetric_clouds_material");
    const auto standardCubeModel = mainAssetManager->loadAsset<ModelAsset>("${enginePath}/Resources/models/standard/cube.obj");

    m_meshBase.setMeshData(standardCubeModel->m_rootNode->findMesh("cube"));
    m_meshBase.setMaterial(standardCloudsMaterial);
}
