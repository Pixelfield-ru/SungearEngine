//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_GLRENDERER_H
#define NATIVECORE_GLRENDERER_H

#include "OpenGL/include/glad/glad.h"

#include "../IRenderer.h"
#include "SGCore/Logging/Log.h"
#include "SGCore/Main/Core.h"

namespace Core::Graphics::API::GL46
{
    class GLRenderer : public IRenderer
    {
    private:
        GLRenderer() noexcept = default;

        //GLRenderer() noexcept = default;

        static inline std::shared_ptr<GLRenderer> m_instance;

    public:
        GLRenderer(const GLRenderer&) = delete;
        GLRenderer(GLRenderer&&) = delete;

        void init(const Main::Window&) noexcept override;

        void checkForErrors(std::source_location location = std::source_location::current()) noexcept override;

        void printInfo() noexcept override;

        void renderFrame() override;

        static const std::shared_ptr<GLRenderer>& getInstance() noexcept;
    };
}

#endif //NATIVECORE_GLRENDERER_H
