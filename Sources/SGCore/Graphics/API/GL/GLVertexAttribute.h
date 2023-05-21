//
// Created by stuka on 21.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GLVERTEXATTRIBUTE_H
#define SUNGEARENGINE_GLVERTEXATTRIBUTE_H

#include <utility>

#include "SGCore/Graphics/API/IVertexAttribute.h"

namespace Core::Graphics::API::GL
{
    class GLVertexAttribute : public IVertexAttribute
    {
    public:
        GLVertexAttribute(int ID, std::string name, int dataType, bool normalized) : IVertexAttribute(ID, std::move(name), dataType, normalized) { }

        int getAttributeSize() noexcept override;
        int getAttributeSizeInBytes() noexcept override;
    };
}

#endif //SUNGEARENGINE_GLVERTEXATTRIBUTE_H
