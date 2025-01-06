//
// Created by stuka on 05.01.2025.
//

#ifndef SUNGEARENGINE_BONE_H
#define SUNGEARENGINE_BONE_H

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Memory/AssetRef.h"

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <string>
#include <vector>

namespace SGCore
{
    struct IMeshData;

    struct BoneVertexWeight
    {
        std::int64_t m_vertexIdx = -1;
        float m_weight = 0;
    };

    // data of copy of bone for each mesh
    struct MeshBoneData
    {
        AssetRef<IMeshData> m_affectedMesh;
        std::vector<BoneVertexWeight> m_weights;
        glm::mat4 m_offsetMatrix = glm::identity<glm::mat4>();
    };

    // i think it must be asset because we can store this bone
    // in custom user code and this bone must to be resolved automatically. assets can do it!
    struct Bone : public IAsset
    {
        sg_serde_as_friend()

        sg_implement_type_id(Bone, 32)

        // MEANS IDX IN m_allBones VECTOR IN PARENT SKELETON
        std::int32_t m_id = -1;
        std::string m_boneName;

        /*std::vector<KeyPosition> m_positionKeys;
        std::vector<KeyRotation> m_rotationKeys;
        std::vector<KeyScale> m_scaleKeys;*/

        // the meshes affected by this bone
        std::vector<MeshBoneData> m_affectedMeshesBoneData;

        std::vector<AssetRef<Bone>> m_children;

    protected:
        /// does nothing!!
        void doLoad(const InterpolatedPath& path) override;

        // todo: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        /// does nothing!!
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;
    };
}

#endif //SUNGEARENGINE_BONE_H
