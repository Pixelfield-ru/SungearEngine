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

        virtual void prepare() = 0;

        virtual IVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType) = 0;
        virtual IVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) = 0;

        virtual IVertexBufferLayout* enableAttribute(const std::shared_ptr<IVertexAttribute>&) = 0;
        virtual IVertexBufferLayout* enableAttributes() = 0;

        virtual IVertexBufferLayout* addAttribute(std::shared_ptr<IVertexAttribute>) noexcept;
        virtual void removeAttribute(const std::shared_ptr<IVertexAttribute>&) noexcept;
    };
}

#endif //SUNGEARENGINE_IVERTEXBUFFERLAYOUT_H
