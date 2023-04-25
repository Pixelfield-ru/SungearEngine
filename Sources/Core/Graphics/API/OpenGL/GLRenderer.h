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
        void init(const Main::Window&) noexcept override;

        void checkForErrors() noexcept override;

        void printInfo() noexcept override;

        void startLoop() override;
    };
}

#endif //NATIVECORE_GLRENDERER_H
