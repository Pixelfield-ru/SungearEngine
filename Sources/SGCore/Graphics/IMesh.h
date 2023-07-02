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

#include "SGCore/Memory/Assets/Materials/PBRMaterial.h"

namespace Core::Graphics
{
    // TODO: make abstract with its own implementation for each GAPI
    // TODO: make render method and material in mesh
    class IMesh
    {
    protected:
        std::shared_ptr<API::IVertexArray> m_vertexArray;

        std::shared_ptr<API::IVertexBuffer> m_positionsBuffer;
        std::shared_ptr<API::IVertexBuffer> m_uvBuffer;
        std::shared_ptr<API::IVertexBuffer> m_normalsBuffer;

        std::shared_ptr<API::IIndexBuffer> m_indicesBuffer;

    public:
        std::string m_name;

        // indices array
        std::vector<std::uint32_t> m_indices;

        // vertices positions array
        std::vector<float> m_positions;

        // uv array
        std::vector<float> m_uv;

        // normals array
        std::vector<float> m_normals;

        std::shared_ptr<Memory::Assets::IMaterial> m_material = std::make_shared<Memory::Assets::PBRMaterial>();

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

        std::shared_ptr<API::IVertexArray> getVertexArray() noexcept;
    };
}

#endif //SUNGEARENGINE_IMESH_H
