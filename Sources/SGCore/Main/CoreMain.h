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

namespace SGCore
{
    class CoreMain
    {
    private:
        static inline Window m_window {};
        static inline std::shared_ptr<IRenderer> m_renderer;

        static inline Timer m_renderTimer { true, 1.0 };
        static inline Timer m_fixedTimer { true, 0.01666 };

    public:
        CoreMain() = delete;

        static void start();

        static void fixedUpdate();
        static void update();

        static Window& getWindow() noexcept;

        static IRenderer& getRenderer() noexcept;

        static Timer& getRenderTimer() noexcept;
        static Timer& getFixedTimer() noexcept;
    };
}

#endif //NATIVECORE_COREMAIN_H
