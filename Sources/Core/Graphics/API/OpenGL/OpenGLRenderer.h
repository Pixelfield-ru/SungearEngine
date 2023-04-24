//
// Created by stuka on 24.04.2023.
//

#ifndef NATIVECORE_OPENGLRENDERER_H
#define NATIVECORE_OPENGLRENDERER_H

#include "../Base.h"
#include "../../../Logging/Log.h"
#include <glad/glad.h>

class OpenGLRenderer : public Core::Graphics::API::Base::IRenderer
{
public:
    void init() override
    {
        Core::Logging::c_printf(Core::Logging::SG_INFO, "OpenGLRenderer initializing...");

        gladLoadGL();

        Core::Logging::c_printf(Core::Logging::SG_INFO, "OpenGLRenderer initialized!");
    }

    void check_for_errors() override
    {
        int err_code = glGetError();
    }
};

#endif //NATIVECORE_OPENGLRENDERER_H
