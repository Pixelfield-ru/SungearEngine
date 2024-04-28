#ifndef NATIVECORE_COREMAIN_H
#define NATIVECORE_COREMAIN_H

#include <sgcore_export.h>
#include <thread>
#include <spdlog/logger.h>

#include "Window.h"
#include "SGUtils/Timer.h"

#include "CoreGlobals.h"
#include "SGUtils/Utils.h"

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
        static inline Event<void()> onInit;
        
        static inline std::string m_pathToSungearEngineSDKSources;
        
        static inline Ref<spdlog::logger> m_defaultLogger;
        
        CoreMain() = delete;

        static void start();

        static Window& getWindow() noexcept;

        static Ref<IRenderer> getRenderer() noexcept;

        static Timer& getRenderTimer() noexcept;
        static Timer& getFixedTimer() noexcept;
        
        static std::uint16_t getFPS() noexcept;
    
    private:
        static inline Window m_window {};
        static inline Ref<IRenderer> m_renderer;
        
        static inline Timer m_renderTimer { true, 1200};
        static inline Timer m_fixedTimer { true, 100.0 };
        
        static void fixedUpdateStart(const double& dt, const double& fixedDt);
        static void fixedUpdateEnd(const double& dt, const double& fixedDt);
        static void updateStart(const double& dt, const double& fixedDt);
        static void updateEnd(const double& dt, const double& fixedDt);
    };
}

#endif //NATIVECORE_COREMAIN_H
