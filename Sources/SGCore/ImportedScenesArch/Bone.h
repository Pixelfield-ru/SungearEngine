//
// Created by stuka on 05.01.2025.
//

#ifndef SUNGEARENGINE_BONE_H
#define SUNGEARENGINE_BONE_H

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Memory/AssetRef.h"
#include "SGCore/Memory/IAssetsRefsResolver.h"

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <string>
#include <vector>

namespace SGCore
{
    class IMeshData;
    struct Skeleton;

    struct BoneVertexWeight
    {
        std::int64_t m_vertexIdx = -1;
        float m_weight = 0;
    };

    // data of copy of bone for each mesh
    struct MeshBoneData
    {
        sg_serde_as_friend()

        sg_assets_refs_resolver_as_friend

        friend struct Bone;

        AssetRef<IMeshData> m_affectedMesh;
        std::vector<BoneVertexWeight> m_weights;

    private:
        AssetsPackage::DataMarkup m_weightsDataMarkupInPackage;
    };

    // i think it must be asset because we can store this bone
    // in custom user code and this bone must to be resolved automatically. assets can do it!
    // BONE MUST BE ATTACHED ONLY TO ONE SKELETON. DO NOT SHARE ONE INSTANCE OF BONE BETWEEN MULTIPLE SKELETONS!!!!
    struct Bone : public IAsset, public IAssetsRefsResolver<Bone>
    {
        sg_serde_as_friend()

        sg_implement_type_id(Bone, 32)

        sg_assets_refs_resolver_as_friend

        friend struct Skeleton;

        // MEANS IDX IN m_allBones VECTOR IN PARENT SKELETON
        std::int32_t m_id = -1;
        glm::mat4 m_offsetMatrix = glm::identity<glm::mat4>();

        glm::vec3 m_currentPosition { 0.0f };

        // the meshes affected by this bone
        std::vector<MeshBoneData> m_affectedMeshesBoneData;

        std::vector<AssetRef<Bone>> m_children;
        Bone* m_parent { };

        const std::string& getName() const noexcept;

    protected:
        /// does nothing!!
        void doLoad(const InterpolatedPath& path) override;

        // todo: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        /// does nothing!!
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept;

    private:
        // can be changed only from skeleton
        std::string m_boneName;
        AssetWeakRef<Skeleton> m_parentSkeleton;
    };
}

#endif //SUNGEARENGINE_BONE_H
