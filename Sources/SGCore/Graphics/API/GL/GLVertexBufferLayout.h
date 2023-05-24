//
// Created by stuka on 22.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GLVERTEXBUFFERLAYOUT_H
#define SUNGEARENGINE_GLVERTEXBUFFERLAYOUT_H

#include "../IVertexBufferLayout.h"
#include "GLVertexAttribute.h"

namespace Core::Graphics::API::GL
{
    class GLVertexBufferLayout : public IVertexBufferLayout
    {
    private:
        std::uint32_t m_stride = 0;

    public:
        GLVertexBufferLayout* prepare() noexcept override;

        std::uint16_t getVertexAttributeSizeInLayout(const SGGDataType&) noexcept;
        std::uint16_t getVertexAttributeSizeInBytes(const SGGDataType&) noexcept;
        std::uint16_t getVertexAttributeAPIDataType(const SGGDataType&) noexcept;

        GLVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType) noexcept override;
        GLVertexAttribute* createVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) noexcept override;

        GLVertexBufferLayout* enableAttribute(const std::shared_ptr<IVertexAttribute>&) noexcept override;
        GLVertexBufferLayout* enableAttributes() noexcept override;

        GLVertexBufferLayout* addAttribute(std::shared_ptr<IVertexAttribute> attribute) noexcept override;
    };
}

#endif //SUNGEARENGINE_GLVERTEXBUFFERLAYOUT_H
