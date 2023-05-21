//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_IINDEXBUFFER_H
#define NATIVECORE_IINDEXBUFFER_H

#include <vector>
#include <glad/glad.h>

namespace Core::Graphics::API
{
    class IIndexBuffer
    {
    public:
        virtual ~IIndexBuffer() = default;

        virtual IIndexBuffer* create() = 0;

        virtual void destroy() = 0;

        virtual IIndexBuffer* putData(std::vector<int> data, bool subData = false) = 0;

        virtual IIndexBuffer* bind() = 0;

        virtual void setUsage(const int&) = 0;
    };
}

#endif //NATIVECORE_IINDEXBUFFER_H
