//
// Created by stuka on 21.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_IVERTEXBUFFERLAYOUT_H
#define SUNGEARENGINE_IVERTEXBUFFERLAYOUT_H

#include <list>
#include <memory>

#include "IVertexAttribute.h"

namespace Core::Graphics::API
{
    class IVertexBufferLayout
    {
    protected:
        std::list<std::shared_ptr<IVertexAttribute>> m_attributes;

    public:
        virtual ~IVertexBufferLayout() = default;

        // prepare vertex buffer layout before enable attributes
        virtual IVertexBufferLayout* prepare() = 0;

        // create new vertex attribute
        virtual IVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType) = 0;
        virtual IVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) = 0;

        // enable attributes (GRAPHICS API SIDE)
        virtual IVertexBufferLayout* enableAttribute(const std::shared_ptr<IVertexAttribute>&) = 0;
        virtual IVertexBufferLayout* enableAttributes() = 0;

        // put attribute
        virtual IVertexBufferLayout* addAttribute(std::shared_ptr<IVertexAttribute>) noexcept;
        virtual void removeAttribute(const std::shared_ptr<IVertexAttribute>&) noexcept;
    };
}

#endif //SUNGEARENGINE_IVERTEXBUFFERLAYOUT_H
