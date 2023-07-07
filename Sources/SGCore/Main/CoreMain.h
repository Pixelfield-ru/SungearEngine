//
// Created by stuka on 17.04.2023.
//

#pragma once

#ifndef NATIVECORE_CORE_H
#define NATIVECORE_CORE_H

#include "SGCore/Logging/Log.h"
#include "Window.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Utils/Timer.h"

namespace Core::Main
{
    class CoreMain
    {
    private:
        static inline Window m_window {};
        static inline std::shared_ptr<Graphics::IRenderer> m_renderer;

        static inline Utils::Timer m_globalTimer { true, 1.0 };

    public:
        CoreMain() = delete;

        static void start();

        static void update();

        static void deltaUpdate(const long double&);

        static Window& getWindow() noexcept;

        static Graphics::IRenderer& getRenderer() noexcept;
    };
}


#endif //NATIVECORE_CORE_H
