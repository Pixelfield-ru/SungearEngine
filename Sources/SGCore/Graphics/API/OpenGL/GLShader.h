//
// Created by stuka on 13.05.2023.
//

#ifndef NATIVECORE_GLSHADER_H
#define NATIVECORE_GLSHADER_H

#include <glad/glad.h>

#include "../Shader.h"

namespace Core::Graphics::API::OpenGL
{
    class GLShader : public Shader
    {
    private:
        GLuint programHandler;

        // vertex, fragment, geometry shaders
        GLuint shadersHandlers[3];

    public:
        void bind() noexcept override;
    };
}

#endif //NATIVECORE_GLSHADER_H
