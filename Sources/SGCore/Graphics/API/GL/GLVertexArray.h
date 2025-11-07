//
// Created by stuka on 24.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GLVERTEXARRAY_H
#define SUNGEARENGINE_GLVERTEXARRAY_H

#include <glad/glad.h>

#include "SGCore/Graphics/API/IVertexArray.h"

namespace SGCore
{
    struct GLVertexArray : IVertexArray
    {
        friend struct GL4Renderer;

        ~GLVertexArray() noexcept override;

        void create() noexcept override;
        void destroy() noexcept override;

        void bind() noexcept override;

    private:
        GLVertexArray() noexcept = default;

        GLuint m_handler = 0;
    };
}

#endif //SUNGEARENGINE_GLVERTEXARRAY_H
