#ifndef NATIVECORE_COREMAIN_H
#define NATIVECORE_COREMAIN_H

#include <sgcore_export.h>

#include "Config.h"
#include "Window.h"
#include "SGCore/Utils/Timer.h"

#include "CoreGlobals.h"
#include "SGCore/Utils/Macroses.h"
#include "SGCore/CrashHandler/HwExceptionHandler.h"

/*namespace Core::Graphics
{
    class IRenderer;
}*/

namespace SGCore
{
    class IRenderer;

    class SGCORE_EXPORT CoreMain
    {
    public:
        static inline Signal<void()> onInit;
        
        CoreMain() = delete;

        static void init();
        static void startCycle() noexcept;

        static Config loadConfig(const std::filesystem::path& configPath);

        static Window& getWindow() noexcept;

        static Ref<IRenderer> getRenderer() noexcept;

        static Timer& getRenderTimer() noexcept;
        static Timer& getFixedTimer() noexcept;
        
        static std::uint16_t getFPS() noexcept;

        SG_NOINLINE static std::filesystem::path getSungearEngineRootPath() noexcept;

        /**
         * Lazy restore of state. Does always in main thread where render cycle is running.
         * @param shouldRestore Should the app restore state?
         */
        static void setShouldRestoreState(bool shouldRestore) noexcept;

        static bool isShouldRestoreState() noexcept;

        /**
         * Immediately restores state of app.
         */
        static void restoreState() noexcept;
    
    private:
        static inline std::filesystem::path s_sungearEngineRootPath;

        static inline Window m_window {};
        static inline Ref<IRenderer> m_renderer;

        static inline std::atomic<bool> m_shouldRestoreState { false };
        
        static inline Timer m_renderTimer { true, 1200};
        static inline Timer m_fixedTimer { true, 100.0 };
        
        static void fixedUpdateStart(const double& dt, const double& fixedDt);
        static void fixedUpdateEnd(const double& dt, const double& fixedDt);
        static void updateStart(const double& dt, const double& fixedDt);
        static void updateEnd(const double& dt, const double& fixedDt);

        static void onFrameBufferResize(Window& window, const int& width, const int& height) noexcept;
    };
}

#endif //NATIVECORE_COREMAIN_H
