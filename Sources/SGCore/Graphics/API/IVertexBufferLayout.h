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
    class IVertexBufferLayout : public std::enable_shared_from_this<IVertexBufferLayout>
    {
    protected:
        std::list<std::shared_ptr<IVertexAttribute>> m_attributes;

    public:
        virtual ~IVertexBufferLayout() = default;

        // prepare vertex buffer layout before enable attributes
        virtual std::shared_ptr<IVertexBufferLayout> prepare() = 0;

        virtual std::shared_ptr<IVertexBufferLayout> reset() = 0;

        // create new vertex attribute
        virtual IVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType) = 0;
        virtual IVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) = 0;

        // enable attributes (GRAPHICS API SIDE)
        virtual std::shared_ptr<IVertexBufferLayout> enableAttribute(const std::shared_ptr<IVertexAttribute>&) = 0;
        virtual std::shared_ptr<IVertexBufferLayout> enableAttributes() = 0;

        // put attribute
        virtual std::shared_ptr<IVertexBufferLayout> addAttribute(std::shared_ptr<IVertexAttribute>) noexcept;
        virtual void removeAttribute(const std::shared_ptr<IVertexAttribute>&) noexcept;
    };
}

#endif //SUNGEARENGINE_IVERTEXBUFFERLAYOUT_H
