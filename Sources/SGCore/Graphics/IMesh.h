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

namespace Core::Graphics
{
    class IMesh
    {
    protected:
        std::shared_ptr<API::IVertexArray> m_vertexArray;

    };
}

#endif //SUNGEARENGINE_IMESH_H
