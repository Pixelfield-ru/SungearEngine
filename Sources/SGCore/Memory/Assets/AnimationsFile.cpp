//
// Created by stuka on 05.01.2025.
//

#include "AnimationsFile.h"
#include <assimp/postprocess.h>

void SGCore::AnimationsFile::doLoad(const SGCore::InterpolatedPath& path)
{
    Assimp::Importer importer;

    const auto resolvedPath = path.resolved();
    const auto utf8ResolvedPath = Utils::toUTF8(resolvedPath.u16string());
    const auto utf8Extension = Utils::toUTF8(resolvedPath.extension().u16string());

    if(importer.IsExtensionSupported(utf8Extension))
    {
        const aiScene* aiImportedScene(importer.ReadFile(utf8ResolvedPath, aiProcess_Triangulate | aiProcess_LimitBoneWeights));

        readFromExistingAssimpScene(*aiImportedScene);
    }
    else
    {
        LOG_E(SGCORE_TAG, "Error loading animations from file '{}': extension '{}' is not supported!",
              utf8ResolvedPath,
              utf8Extension);
    }
}

void SGCore::AnimationsFile::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{

}

void SGCore::AnimationsFile::readFromExistingAssimpScene(const aiScene& scene) noexcept
{
    for(std::uint32_t i = 0; i < scene.mNumAnimations; ++i)
    {
        const auto* curAnimation = scene.mAnimations[i];

        // curAnimation->mMeshChannels[0]->
        // for(std::uint32_t j = 0; j < curAnimation->mNumChannels)
    }
}
