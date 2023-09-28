//
// Created by stuka on 17.04.2023.
//

#ifndef NATIVECORE_COREMAIN_H
#define NATIVECORE_COREMAIN_H

#include "SGCore/Logging/Log.h"
#include "Window.h"
#include "SGCore/Utils/Timer.h"

#include "SGCore/Graphics/API/IRenderer.h"

/*namespace Core::Graphics
{
    class IRenderer;
}*/

namespace Core::Main
{
    class CoreMain
    {
    private:
        static inline Window m_window {};
        static inline std::shared_ptr<Graphics::IRenderer> m_renderer;

        static inline Utils::Timer m_renderTimer {true, 1.0 };
        static inline Utils::Timer m_fixedTimer {true, 1.0 };

    public:
        CoreMain() = delete;

        static void start();

        static void FPSNotRelativeFixedUpdate();
        static void FPSRelativeFixedUpdate();

        static void deltaUpdate(const long double&);

        static Window& getWindow() noexcept;

        static Graphics::IRenderer& getRenderer() noexcept;
    };
}

#endif //NATIVECORE_COREMAIN_H
