//
// Created by stuka on 21.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GLVERTEXATTRIBUTE_H
#define SUNGEARENGINE_GLVERTEXATTRIBUTE_H

#include <utility>

#include "SGCore/Graphics/API/IVertexAttribute.h"
//#include "GLVertexBufferLayout.h"

namespace Core::Graphics::API::GL
{
    class GLVertexAttribute : public IVertexAttribute
    {
        friend class GLVertexBufferLayout;

    public:
        GLVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) : IVertexAttribute(ID, std::move(name), dataType, normalized) { }
    };
}

#endif //SUNGEARENGINE_GLVERTEXATTRIBUTE_H
