//
// Created by stuka on 07.05.2023.
//

#ifndef NATIVECORE_MODELASSET_H
#define NATIVECORE_MODELASSET_H

#include <SGCore/pch.h>

#include "IAsset.h"

#include "SGCore/ImportedScenesArch/Node.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "Skeleton.h"

namespace SGCore
{
    class ModelAsset : public IAsset, public IAssetsRefsResolver<ModelAsset>
    {
    public:
        sg_serde_as_friend()

        sg_implement_type_id(ModelAsset, 8)

        sg_assets_refs_resolver_as_friend

        Ref<Node> m_rootNode;

    protected:
        void doLoad(const InterpolatedPath& path) override;
        void doLazyLoad() override;
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;

        // todo: impl
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept SG_CRTP_OVERRIDE;

    private:
        // this is so fucking shitty... but assimp forces me to do this
        struct BoneHierarchyNode
        {
            std::string m_name;
            // bones with that name. IT IS GUARANTEED THAT m_aiBones.size() == m_affectedMeshes.size() AFTER COLLECTING BONES
            std::vector<const aiBone*> m_aiBones { };
            // the meshes affected by this bone. IT IS GUARANTEED THAT m_aiBones.size() == m_affectedMeshes.size() AFTER COLLECTING BONES
            std::vector<const aiMesh*> m_affectedMeshes { };
            std::vector<std::int32_t> m_children;
            std::int32_t m_parent = -1;
        };

        // local import flags
        // TODO: maybe reimport after change flags
        int m_importerFlags = 0;

        // model name
        std::string m_modelName;

        void processNode(const aiNode*, const aiScene*, std::shared_ptr<Node>& outputNode);
        AssetRef<IMeshData> processMesh(aiMesh*, const aiScene*);
        void loadTextures(aiMaterial* aiMat, AssetRef<IMaterial>& sgMaterial, const aiTextureType& aiTexType, const SGTextureType& sgMaterialTextureType);

        void prepareNodeMeshes(const Ref<Node>& node) noexcept;

        // FOR BUILDING SKELETONS ==================================================

        std::vector<AssetRef<Skeleton>> processSkeletons(const aiScene* fromScene) noexcept;
        // sub process of processSkeletons function
        void collectAllBones(std::unordered_map<std::string, BoneHierarchyNode>& bones, const aiScene* scene) noexcept;
        // sub process of processSkeletons function
        void buildBonesHierarchy(const aiScene* scene,
                                        std::vector<BoneHierarchyNode>& bones) noexcept;
        void initAndAddBoneToSkeleton(AssetRef<Bone>& skeletonBone,
                                      const BoneHierarchyNode& tmpBone,
                                      std::vector<BoneHierarchyNode>& hierarchyBones,
                                      const AssetRef<Skeleton>& toSkeleton) noexcept;

        static int32_t findBoneIndex(const std::string& name,
                                     const std::vector<BoneHierarchyNode>& bones) noexcept;

        static std::int32_t findParentNodeWithBone(const aiNode* currentParentNode, const std::vector<BoneHierarchyNode>& fromBones) noexcept;

        static void trimAndSetupWeightsOfMeshes(const AssetRef<Bone>& currentSkeletonBone) noexcept;
    };
}

#endif //NATIVECORE_MODELASSET_H
