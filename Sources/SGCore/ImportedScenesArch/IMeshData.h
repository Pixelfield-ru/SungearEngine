#ifndef SUNGEARENGINE_IMESH_H
#define SUNGEARENGINE_IMESH_H

#include <SGCore/pch.h>
#include "SGCore/Memory/IAssetsRefsResolver.h"

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Math/AABB.h"
#include "SGCore/Serde/Defines.h"
#include "SGCore/Memory/AssetRefFromThis.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Main/CoreMain.h"
#include "Vertex.h"
#include "Bone.h"

sg_predeclare_serde()

namespace SGCore
{
    class IVertexBuffer;
    class IVertexArray;
    class IIndexBuffer;

    class IMaterial;

    class Scene;

    class AssetManager;

    struct VertexColorsSet
    {
        sg_serde_as_friend()

        friend class IMeshData;

        std::vector<float> m_colors;

    private:
        std::streamsize m_offsetInPackage = 0;
        std::streamsize m_sizeInPackage = 0;
    };

    class IMeshData : public IAsset, public AssetRefFromThis<IMeshData>, public IAssetsRefsResolver<IMeshData>
    {
    public:
        sg_serde_as_friend()

        sg_implement_type_id(IMeshData, 13)

        sg_assets_refs_resolver_as_friend

        friend class AssetManager;
        friend class Node;

        AABB<> m_aabb;
        
        // Mesh() noexcept;
        virtual ~IMeshData() override = default;

        IMeshData();
        IMeshData(const IMeshData&) = default;
        IMeshData(IMeshData&&) noexcept = default;

        std::string m_name;

        // indices array
        std::vector<std::uint32_t> m_indices;

        std::vector<Vertex> m_vertices;
        std::vector<BoneVertexWeight> m_tmpVertexWeights;

        // vertices positions array
        /*std::vector<float> m_positions;

        // uv array
        std::vector<float> m_uv;

        // normals array
        std::vector<float> m_normals;

        // tangents array
        std::vector<float> m_tangents;

        // bitangents array
        std::vector<float> m_bitangents;*/

        // sets (usually 8 sets) of colors for every vertex
        std::vector<VertexColorsSet> m_verticesColors;

        AssetRef<IMaterial> m_material;
        
        Ref<btTriangleMesh> m_physicalMesh;

        // ----------------
        virtual void prepare();

        /**
         * Destroys/clears all buffers of mesh on GPU and CPU. Settings saved.
         */
        virtual void destroy() noexcept;

        void setVertexPosition(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept;
        void getVertexPosition(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept;

        void setVertexUV(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept;
        void getVertexUV(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept;

        void setVertexNormal(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept;
        void getVertexNormal(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept;

        void setIndex(const std::uint64_t& faceIdx, const std::uint64_t& indexIdx, const std::uint64_t& value) noexcept;
        void getFaceIndices(const std::uint64_t& faceIdx, std::uint64_t& outIdx0, std::uint64_t& outIdx1, std::uint64_t& outIdx2) noexcept;

        void setData(const AssetRef<IMeshData>& other) noexcept;

        ECS::entity_t addOnScene(const Ref<Scene>& scene,
                                 const std::string& layerName) noexcept;

        /**
         * Moves all textures of the current material to the new material and sets the new material as the current one.
         * @see Core::Memory::Assets::IMaterial::copyTexturesRefs
         * @param[in] newMaterial The material to which the textures will be moved and which will be set as the current one.
         */
        void migrateAndSetNewMaterial(const AssetRef<IMaterial>& newMaterial) noexcept;
        
        template<typename VScalarT, typename IScalarT>
        static Ref<btTriangleMesh> generatePhysicalMesh(const std::vector<glm::vec<3, VScalarT>>& vertices, const std::vector<IScalarT>& indices) noexcept
        {
            auto physicalMesh = MakeRef<btTriangleMesh>();
            
            for(size_t i = 0; i < indices.size(); i += 3)
            {
                size_t ti0 = indices[i] * 3;
                size_t ti1 = indices[i + 1] * 3;
                size_t ti2 = indices[i + 2] * 3;
                
                physicalMesh->addTriangle(btVector3(vertices[ti0].x, vertices[ti0].y, vertices[ti0].z),
                                          btVector3(vertices[ti1].x, vertices[ti1].y, vertices[ti1].z),
                                          btVector3(vertices[ti2].x, vertices[ti2].y, vertices[ti2].z));
            }
            
            return physicalMesh;
        }
        
        void generatePhysicalMesh() noexcept;

        Ref<IVertexArray> getVertexArray() const noexcept;

    protected:
        void doLoad(const InterpolatedPath& path) override;
        void doLazyLoad() override;
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;
        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept SG_CRTP_OVERRIDE;

        std::streamsize m_indicesOffsetInPackage = 0;
        std::streamsize m_indicesSizeInPackage = 0;

        std::streamsize m_verticesOffsetInPackage = 0;
        std::streamsize m_verticesSizeInPackage = 0;

        // ========================================================================================
        // ========================================================================================

        Ref<IVertexArray> m_vertexArray;

        Ref<IVertexBuffer> m_verticesBuffer;
        /*Ref<IVertexBuffer> m_positionsBuffer;
        Ref<IVertexBuffer> m_uvBuffer;
        Ref<IVertexBuffer> m_normalsBuffer;
        Ref<IVertexBuffer> m_tangentsBuffer;
        Ref<IVertexBuffer> m_bitangentsBuffer;*/
        std::vector<Ref<IVertexBuffer>> m_verticesColorsBuffers;

        Ref<IIndexBuffer> m_indicesBuffer;

        template<typename... AssetCtorArgs>
        static Ref<IMeshData> createRefInstance(AssetCtorArgs&&... assetCtorArgs) noexcept
        {
            auto meshData = Ref<IMeshData>(CoreMain::getRenderer()->createMeshData(std::forward<AssetCtorArgs>(assetCtorArgs)...));

            return meshData;
        }
    };
}

#endif //SUNGEARENGINE_IMESH_H
