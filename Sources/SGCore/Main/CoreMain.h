#ifndef NATIVECORE_COREMAIN_H
#define NATIVECORE_COREMAIN_H

#include "Window.h"
#include "SGUtils/Timer.h"

#include "CoreGlobals.h"

/*namespace Core::Graphics
{
    class IRenderer;
}*/

namespace SGCore
{
    class IRenderer;

    class CoreMain
    {
    private:
        static inline Window m_window {};
        static inline Ref<IRenderer> m_renderer;

        static inline SGUtils::Timer m_renderTimer { true, 1.0 };
        static inline SGUtils::Timer m_fixedTimer { true, 100.0 };

    public:
        CoreMain() = delete;

        static void start();

        static void fixedUpdate(const double& dt, const double& fixedDt);
        static void update(const double& dt);

        static Window& getWindow() noexcept;

        static Ref<IRenderer> getRenderer() noexcept;

        static SGUtils::Timer& getRenderTimer() noexcept;
        static SGUtils::Timer& getFixedTimer() noexcept;
    };
}

#endif //NATIVECORE_COREMAIN_H
