//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_GLRENDERER_H
#define NATIVECORE_GLRENDERER_H

#include <glad/glad.h>

#include "../IRenderer.h"
#include "../../../Logging/Log.h"
#include "../../../Memory/Resource.h"
#include "../../../Main/Core.h"

namespace Core::Graphics::API::OpenGL
{
    class GLRenderer : public IRenderer
    {
    public:
        void init(const Main::Window& wnd) noexcept override;

        void check_for_errors() noexcept override;

        void print_info() noexcept override;

        void start_loop() override;
    };
}

#endif //NATIVECORE_GLRENDERER_H
