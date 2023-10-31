//
// Created by stuka on 20.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_IMESH_H
#define SUNGEARENGINE_IMESH_H

#include <memory>
#include <list>

#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IIndexBuffer.h"

#include "SGCore/Memory/Assets/Materials/IMaterial.h"

namespace Core::ImportedScene
{
    class IMesh
    {
    protected:
        std::shared_ptr<Graphics::IVertexArray> m_vertexArray;

        std::shared_ptr<Graphics::IVertexBuffer> m_positionsBuffer;
        std::shared_ptr<Graphics::IVertexBuffer> m_uvBuffer;
        std::shared_ptr<Graphics::IVertexBuffer> m_normalsBuffer;
        std::shared_ptr<Graphics::IVertexBuffer> m_tangentsBuffer;
        std::shared_ptr<Graphics::IVertexBuffer> m_bitangentsBuffer;

        std::shared_ptr<Graphics::IIndexBuffer> m_indicesBuffer;

    public:
        SGDrawMode m_drawMode = SGDrawMode::SGG_TRIANGLES;

        IMesh() noexcept;
        virtual ~IMesh() = default;

        std::string m_name;

        bool m_useIndices = true;

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

        std::shared_ptr<Memory::Assets::IMaterial> m_material;

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

        /**
         * Moves all textures of the current material to the new material and sets the new material as the current one.
         * @see Core::Memory::Assets::IMaterial::copyTextures
         * @param[in] newMaterial The material to which the textures will be moved and which will be set as the current one.
         */
        void migrateAndSetNewMaterial(const std::shared_ptr<Memory::Assets::IMaterial>& newMaterial) noexcept;

        std::shared_ptr<Graphics::IVertexArray> getVertexArray() noexcept;
    };
}

#endif //SUNGEARENGINE_IMESH_H
