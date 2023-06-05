//
// Created by stuka on 24.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GLVERTEXARRAY_H
#define SUNGEARENGINE_GLVERTEXARRAY_H

#include "../IVertexArray.h"

#include <glad/glad.h>

namespace Core::Graphics::API::GL
{
    class GLVertexArray : public IVertexArray
    {
    private:
        GLuint m_handler = 0;

    public:
        ~GLVertexArray() noexcept override;

        std::shared_ptr<IVertexArray> create() noexcept override;
        void destroy() noexcept override;

        std::shared_ptr<IVertexArray> bind() noexcept override;
    };
}

#endif //SUNGEARENGINE_GLVERTEXARRAY_H
