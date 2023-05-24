//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_IVERTEXARRAY_H
#define NATIVECORE_IVERTEXARRAY_H

namespace Core::Graphics::API
{
    class IVertexArray
    {
    public:
        virtual ~IVertexArray() = default;

        virtual IVertexArray* create() = 0;
        virtual void destroy() = 0;

        virtual IVertexArray* bind() = 0;
    };
}

#endif //NATIVECORE_IVERTEXARRAY_H
