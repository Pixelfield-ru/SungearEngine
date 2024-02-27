//
// Created by stuka on 20.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_IMESH_H
#define SUNGEARENGINE_IMESH_H

#include <memory>
#include <vector>
#include <cstdint>
#include <string>
#include <glm/glm.hpp>
#include <entt/entity/entity.hpp>
#include <BulletCollision/CollisionShapes/btTriangleMesh.h>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IVertexBuffer;
    class IVertexArray;
    class IIndexBuffer;

    class IMaterial;

    class Scene;
    
    class IMeshData : public std::enable_shared_from_this<IMeshData>
    {
    protected:
        Ref<IVertexArray> m_vertexArray;

        Ref<IVertexBuffer> m_positionsBuffer;
        Ref<IVertexBuffer> m_uvBuffer;
        Ref<IVertexBuffer> m_normalsBuffer;
        Ref<IVertexBuffer> m_tangentsBuffer;
        Ref<IVertexBuffer> m_bitangentsBuffer;

        Ref<IIndexBuffer> m_indicesBuffer;

    public:
        glm::vec3 m_aabbMin { 0.0 };
        glm::vec3 m_aabbMax { 0.0 };
        
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
        
        entt::entity addOnScene(const Ref<Scene>& scene,
                                const std::string& layerName) noexcept;

        /**
         * Moves all textures of the current material to the new material and sets the new material as the current one.
         * @see Core::Memory::Assets::IMaterial::copyTextures
         * @param[in] newMaterial The material to which the textures will be moved and which will be set as the current one.
         */
        void migrateAndSetNewMaterial(const Ref<IMaterial>& newMaterial) noexcept;
        
        void generatePhysicalMesh() noexcept;

        Ref<IVertexArray> getVertexArray() noexcept;
    };
}

#endif //SUNGEARENGINE_IMESH_H
