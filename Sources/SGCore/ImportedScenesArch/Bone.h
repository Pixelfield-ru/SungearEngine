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

    struct BoneVertexWeight
    {
        std::int64_t m_vertexIdx = -1;
        float m_weight = 0;
    };

    // data of copy of bone for each mesh
    struct MeshBoneData
    {
        sg_serde_as_friend()

        friend struct Bone;

        AssetRef<IMeshData> m_affectedMesh;
        std::vector<BoneVertexWeight> m_weights;

    private:
        AssetsPackage::DataMarkup m_weightsDataMarkupInPackage;
    };

    // i think it must be asset because we can store this bone
    // in custom user code and this bone must to be resolved automatically. assets can do it!
    struct Bone : public IAsset, public IAssetsRefsResolver<Bone>
    {
        sg_serde_as_friend()

        sg_implement_type_id(Bone, 32)

        sg_assets_refs_resolver_as_friend

        // MEANS IDX IN m_allBones VECTOR IN PARENT SKELETON
        std::int32_t m_id = -1;
        std::string m_boneName;
        glm::mat4 m_offsetMatrix = glm::identity<glm::mat4>();

        glm::vec3 m_currentPosition { 0.0f };

        // the meshes affected by this bone
        std::vector<MeshBoneData> m_affectedMeshesBoneData;

        std::vector<AssetRef<Bone>> m_children;
        // todo: make parent bone resolving (loading) when scene loaded
        AssetWeakRef<Bone> m_parent;

    protected:
        /// does nothing!!
        void doLoad(const InterpolatedPath& path) override;

        // todo: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        /// does nothing!!
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept SG_CRTP_OVERRIDE;
    };
}

#endif //SUNGEARENGINE_BONE_H
