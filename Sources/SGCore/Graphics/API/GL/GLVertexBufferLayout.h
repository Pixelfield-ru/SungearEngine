//
// Created by stuka on 22.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GLVERTEXBUFFERLAYOUT_H
#define SUNGEARENGINE_GLVERTEXBUFFERLAYOUT_H

#include "../IVertexBufferLayout.h"
#include "GLVertexAttribute.h"

namespace SGCore
{
    class GLVertexBufferLayout : public IVertexBufferLayout
    {
    private:
        std::uint32_t m_stride = 0;

    public:
        Ref<IVertexBufferLayout> prepare() noexcept override;

        Ref<IVertexBufferLayout> reset() noexcept final;

        static std::uint16_t getVertexAttributeScalarsCount(const SGGDataType& dataType) noexcept;

        GLVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType) noexcept override;
        GLVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) noexcept override;
        GLVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized, const size_t& divisor) noexcept override;
        GLVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, const size_t& divisor) noexcept override;
        GLVertexAttribute*
        createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType,
                              const std::uint16_t& scalarsCount, bool normalized, const size_t& stride,
                              const size_t& offset, const size_t& divisor) noexcept override;

        Ref<IVertexBufferLayout> enableAttribute(const Ref<IVertexAttribute>&) noexcept override;
        Ref<IVertexBufferLayout> enableAttributes() noexcept override;

        Ref<IVertexBufferLayout> addAttribute(Ref<IVertexAttribute> attribute) noexcept override;
    };
}

#endif //SUNGEARENGINE_GLVERTEXBUFFERLAYOUT_H
