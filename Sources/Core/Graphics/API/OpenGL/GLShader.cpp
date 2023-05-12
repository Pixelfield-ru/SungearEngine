//
// Created by stuka on 13.05.2023.
//

#include "GLShader.h"

void Core::Graphics::API::OpenGL::GLShader::bind() noexcept
{
    glUseProgram(programHandler);
}
