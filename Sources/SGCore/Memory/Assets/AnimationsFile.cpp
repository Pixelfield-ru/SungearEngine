//
// Created by stuka on 05.01.2025.
//

#include "AnimationsFile.h"
#include "SGCore/Utils/AssimpUtils.h"

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
    LOG_NOT_IMPLEMENTED(SGCORE_TAG);
}

void SGCore::AnimationsFile::doReloadFromDisk(SGCore::AssetsLoadPolicy loadPolicy,
                                              SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{
    LOG_NOT_IMPLEMENTED(SGCORE_TAG);
}

void SGCore::AnimationsFile::onMemberAssetsReferencesResolveImpl(SGCore::AssetManager* updatedAssetManager) noexcept
{
    LOG_NOT_IMPLEMENTED(SGCORE_TAG);
}

void SGCore::AnimationsFile::readFromExistingAssimpScene(const aiScene& scene) noexcept
{
    auto parentAssetManager = getParentAssetManager();

    for(std::uint32_t i = 0; i < scene.mNumAnimations; ++i)
    {
        const auto* curAnimation = scene.mAnimations[i];

        // curAnimation contains skeletal animation
        if(curAnimation->mNumChannels > 0)
        {
            auto skeletalAnimation = parentAssetManager->getOrAddAssetByPath<SkeletalAnimationAsset>(
                    getPath() / "skeletal" / std::string(curAnimation->mName.C_Str()));

            skeletalAnimation->m_animationName = curAnimation->mName.C_Str();

            for(size_t j = 0; j < curAnimation->mNumChannels; ++j)
            {
                const auto* nodeAnim = curAnimation->mChannels[j];

                SkeletalBoneAnimation skeletalBoneAnimation;
                skeletalBoneAnimation.m_boneName = nodeAnim->mNodeName.C_Str();

                for(size_t k = 0; k < nodeAnim->mNumPositionKeys; ++k)
                {
                    const auto& aiPosKey = nodeAnim->mPositionKeys[k];

                    KeyPosition keyPosition;
                    keyPosition.m_position = AssimpUtils::aiToGLM(aiPosKey.mValue);
                    keyPosition.m_timeStamp = aiPosKey.mTime;

                    skeletalBoneAnimation.m_positionKeys.push_back(keyPosition);
                }

                for(size_t k = 0; k < nodeAnim->mNumRotationKeys; ++k)
                {
                    const auto& aiPosKey = nodeAnim->mRotationKeys[k];

                    KeyRotation keyRotation;
                    keyRotation.m_rotation = AssimpUtils::aiToGLM(aiPosKey.mValue);
                    keyRotation.m_timeStamp = aiPosKey.mTime;

                    skeletalBoneAnimation.m_rotationKeys.push_back(keyRotation);
                }

                for(size_t k = 0; k < nodeAnim->mNumScalingKeys; ++k)
                {
                    const auto& aiPosKey = nodeAnim->mScalingKeys[k];

                    KeyScale keyScale;
                    keyScale.m_scale = AssimpUtils::aiToGLM(aiPosKey.mValue);
                    keyScale.m_timeStamp = aiPosKey.mTime;

                    skeletalBoneAnimation.m_scaleKeys.push_back(keyScale);
                }

                skeletalAnimation->m_bonesAnimations.push_back(skeletalBoneAnimation);
            }
        }
    }
}
