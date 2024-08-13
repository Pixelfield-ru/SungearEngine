//
// Created by stuka on 21.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_IVERTEXBUFFERLAYOUT_H
#define SUNGEARENGINE_IVERTEXBUFFERLAYOUT_H

#include <SGCore/pch.h>

#include "IVertexAttribute.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IVertexBufferLayout : public std::enable_shared_from_this<IVertexBufferLayout>
    {
    protected:
        std::list<Ref<IVertexAttribute>> m_attributes;

    public:
        virtual ~IVertexBufferLayout() = default;

        // prepare vertex buffer layout before enable attributes
        virtual Ref<IVertexBufferLayout> prepare() = 0;

        virtual Ref<IVertexBufferLayout> reset() = 0;

        // create new vertex attribute
        virtual IVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType) = 0;
        virtual IVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) = 0;
        virtual IVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized, const size_t& divisor) = 0;
        virtual IVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, const size_t& divisor) = 0;
        virtual IVertexAttribute*
        createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType,
                              const std::uint16_t& scalarsCount, bool normalized, const size_t& stride,
                              const size_t& offset, const size_t& divisor) = 0;
        
        // enable attributes (GRAPHICS API SIDE)
        virtual Ref<IVertexBufferLayout> enableAttribute(const Ref<IVertexAttribute>&) = 0;
        virtual Ref<IVertexBufferLayout> enableAttributes() = 0;

        // put attribute
        virtual Ref<IVertexBufferLayout> addAttribute(Ref<IVertexAttribute>) noexcept;
        virtual void removeAttribute(const Ref<IVertexAttribute>&) noexcept;
    };
}

#endif //SUNGEARENGINE_IVERTEXBUFFERLAYOUT_H
