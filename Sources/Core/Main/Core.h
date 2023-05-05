//
// Created by stuka on 17.04.2023.
//

#pragma once

#ifndef NATIVECORE_CORE_H
#define NATIVECORE_CORE_H

#include "../Graphics/API/IRenderer.h"
#include "../Utils/Timer.h"

namespace Core::Main
{
    class Core
    {
    private:
        static inline Window m_window{};
        static inline Graphics::API::IRenderer* m_renderer;

        static inline Utils::Timer globalTimer { true };
    public:
        Core() = delete;

        static void start();

        static Window& getWindow() noexcept;

        static Graphics::API::IRenderer& getRenderer() noexcept;
    };
}


#endif //NATIVECORE_CORE_H
