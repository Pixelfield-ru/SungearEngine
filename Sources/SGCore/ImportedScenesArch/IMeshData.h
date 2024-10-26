#ifndef SUNGEARENGINE_IMESH_H
#define SUNGEARENGINE_IMESH_H

#include <SGCore/pch.h>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Math/AABB.h"

namespace SGCore
{
    class IVertexBuffer;
    class IVertexArray;
    class IIndexBuffer;

    class IMaterial;

    class Scene;

    class IMeshData : public std::enable_shared_from_this<IMeshData>
    {
    public:
        AABB<> m_aabb;
        
        // Mesh() noexcept;
        virtual ~IMeshData() = default;

        IMeshData();
        IMeshData(const IMeshData&) = default;
        IMeshData(IMeshData&&) noexcept = default;

        std::string m_name;

        // indices array
        std::vector<std::uint32_t> m_indices;

        // vertices positions array
        std::vector<float> m_positions;

        // uv array
        std::vector<float> m_uv;

        // normals array
        std::vector<float> m_normals;

        // tangents array
        std::vector<float> m_tangents;

        // bitangents array
        std::vector<float> m_bitangents;

        Ref<IMaterial> m_material;
        
        Ref<btTriangleMesh> m_physicalMesh;

        // ----------------
        virtual void prepare() = 0;

        void setVertexPosition(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept;
        void getVertexPosition(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept;

        void setVertexUV(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept;
        void getVertexUV(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept;

        void setVertexNormal(const std::uint64_t& vertexIdx, const float& x, const float& y, const float& z) noexcept;
        void getVertexNormal(const std::uint64_t& vertexIdx, float& outX, float& outY, float& outZ) noexcept;

        void setIndex(const std::uint64_t& faceIdx, const std::uint64_t& indexIdx, const std::uint64_t& value) noexcept;
        void getFaceIndices(const std::uint64_t& faceIdx, std::uint64_t& outIdx0, std::uint64_t& outIdx1, std::uint64_t& outIdx2) noexcept;

        void setData(const Ref<IMeshData>& other) noexcept;
        
        entity_t addOnScene(const Ref<Scene>& scene,
                            const std::string& layerName) noexcept;

        /**
         * Moves all textures of the current material to the new material and sets the new material as the current one.
         * @see Core::Memory::Assets::IMaterial::copyTextures
         * @param[in] newMaterial The material to which the textures will be moved and which will be set as the current one.
         */
        void migrateAndSetNewMaterial(const Ref<IMaterial>& newMaterial) noexcept;
        
        template<typename VScalarT, typename IScalarT>
        static Ref<btTriangleMesh> generatePhysicalMesh(const std::vector<VScalarT>& vertices, const std::vector<IScalarT>& indices) noexcept
        {
            auto physicalMesh = MakeRef<btTriangleMesh>();
            
            for(size_t i = 0; i < indices.size(); i += 3)
            {
                size_t ti0 = indices[i] * 3;
                size_t ti1 = indices[i + 1] * 3;
                size_t ti2 = indices[i + 2] * 3;
                
                physicalMesh->addTriangle(btVector3(vertices[ti0], vertices[ti0 + 1], vertices[ti0 + 2]),
                                          btVector3(vertices[ti1], vertices[ti1 + 1], vertices[ti1 + 2]),
                                          btVector3(vertices[ti2], vertices[ti2 + 1], vertices[ti2 + 2]));
            }
            
            return physicalMesh;
        }
        
        void generatePhysicalMesh() noexcept;

        Ref<IVertexArray> getVertexArray() noexcept;

    protected:
        Ref<IVertexArray> m_vertexArray;

        Ref<IVertexBuffer> m_positionsBuffer;
        Ref<IVertexBuffer> m_uvBuffer;
        Ref<IVertexBuffer> m_normalsBuffer;
        Ref<IVertexBuffer> m_tangentsBuffer;
        Ref<IVertexBuffer> m_bitangentsBuffer;

        Ref<IIndexBuffer> m_indicesBuffer;
    };
}

#endif //SUNGEARENGINE_IMESH_H
