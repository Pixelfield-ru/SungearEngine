//
// Created by stuka on 07.05.2023.
//

#pragma once

#ifndef NATIVECORE_MODELASSET_H
#define NATIVECORE_MODELASSET_H

#include "IAsset.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IIndexBuffer.h"

namespace Core::Memory::Assets
{
    class ModelAsset : public IAsset, public std::enable_shared_from_this<ModelAsset>
    {
    private:
        std::shared_ptr<Graphics::API::IVertexArray> m_vertexArray;

        std::shared_ptr<Graphics::API::IVertexBuffer> m_vPositionsBuffer;
        std::shared_ptr<Graphics::API::IVertexBuffer> m_uvBuffer;
        std::shared_ptr<Graphics::API::IVertexBuffer> m_normalsBuffer;

        std::shared_ptr<Graphics::API::IIndexBuffer> m_indexBuffer;

    public:
        std::shared_ptr<IAsset> load(const std::string_view&) override;
    };
}

#endif //NATIVECORE_MODELASSET_H
