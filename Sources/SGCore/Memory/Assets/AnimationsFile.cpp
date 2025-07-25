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
        const aiScene* aiImportedScene(importer.ReadFile(utf8ResolvedPath, aiProcess_Triangulate));

        if(!aiImportedScene || aiImportedScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiImportedScene->mRootNode)
        {
            LOG_E(SGCORE_TAG,
                  "Assimp error (while importing scene): {}\n{}",
                  importer.GetErrorString(),
                  SG_CURRENT_LOCATION_STR);
            return;
        }

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
    std::cout << "AnimationsFile::doLoadFromBinaryFile: skeletal animations count: " << m_skeletalAnimations.size() << std::endl;

    for(const auto& skeletalAnimation : m_skeletalAnimations)
    {
        skeletalAnimation->doLoadFromBinaryFile(parentAssetManager);
    }
}

void SGCore::AnimationsFile::doReloadFromDisk(SGCore::AssetsLoadPolicy loadPolicy,
                                              SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{
    LOG_NOT_IMPLEMENTED(SGCORE_TAG);
}

void SGCore::AnimationsFile::onMemberAssetsReferencesResolveImpl(SGCore::AssetManager* updatedAssetManager) noexcept
{
    for(auto& animation : m_skeletalAnimations)
    {
        AssetManager::resolveAssetReference(updatedAssetManager, animation);
    }
}

void SGCore::AnimationsFile::readFromExistingAssimpScene(const aiScene& scene) noexcept
{
    if(scene.mNumAnimations == 0) return;

    auto parentAssetManager = getParentAssetManager();

    m_skeletalAnimations.clear();

    for(std::uint32_t i = 0; i < scene.mNumAnimations; ++i)
    {
        const auto* curAnimation = scene.mAnimations[i];

        // curAnimation contains skeletal animation
        if(curAnimation->mNumChannels > 0)
        {
            auto skeletalAnimation = parentAssetManager->getOrAddAssetByPath<SkeletalAnimationAsset>(
                    getPath() / "skeletal" / std::string(curAnimation->mName.C_Str()));

            skeletalAnimation->m_animationName = curAnimation->mName.C_Str();
            skeletalAnimation->m_duration = curAnimation->mDuration;
            skeletalAnimation->m_ticksPerSecond = curAnimation->mTicksPerSecond;
            if(skeletalAnimation->m_ticksPerSecond == 0.0f)
            {
                skeletalAnimation->m_ticksPerSecond = 1.0f;
            }

            for(size_t j = 0; j < curAnimation->mNumChannels; ++j)
            {
                const auto* nodeAnim = curAnimation->mChannels[j];

                SkeletalBoneAnimation& skeletalBoneAnimation = skeletalAnimation->m_bonesAnimations[nodeAnim->mNodeName.C_Str()];
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
            }

            m_skeletalAnimations.push_back(skeletalAnimation);
        }
    }
}
