#ifndef SUNGEARENGINE_PROGRAMWINDOW_H
#define SUNGEARENGINE_PROGRAMWINDOW_H

#include <sgcore_export.h>

#include "SGCore/Utils/Platform.h"

#include "SGCore/Utils/Signal.h"

#include <string>

#if SG_PLATFORM_OS_ANDROID
#include <jni.h>
#include <android/native_window.h>
#include <EGL/egl.h>
#endif

#if SG_PLATFORM_OS_WINDOWS
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#endif

#if SG_PLATFORM_OS_LINUX
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#endif


#include "SGCore/ExternalAPI/Java/Main.h"

namespace SGCore
{
    struct Window;

    /**
     * Note that the config changes dynamically.
     */
     // TODO: make config save
    class WindowConfig
    {
        friend struct Window;
        
        int m_sizeX = 500;
        int m_sizeY = 250;

        /**
         * Window minimum X size limit.
         */
        int m_sizeMinLimitX = -1;
        /**
         * Window minimum Y size limit.
         */
        int m_sizeMinLimitY = -1;

        /**
         * Window maximum X size limit.
         */
        int m_sizeMaxLimitX = -1;
        /**
         * Window maximum Y size limit.
         */
        int m_sizeMaxLimitY = -1;

        /**
         * Window X position.
         */
        int m_positionX = 100;
        /**
         * Window Y position.
         */
        int m_positionY = 100;

        /**
         * Window title.
         */
        std::string m_title = "Powered by Core";

        // TODO: fix delta time
        /**
         * Window will use vertical synchronization
         */
        bool m_swapInterval = false;

        bool m_enableStickyKeys = false;

        // on window create settings ------------------------------
        /**
         * Window will be centralized on the monitor.
         */
        bool m_centralizeWindow = true;

        /**
         * Window will use half of the main monitor.
         */
        bool m_useHalfMonitor = true;

        /**
         * The cursor will be hidden and will be centralized in the window.
         */
        bool m_hideAndCentralizeCursor = false;
        // --------------------------------------------------------

        bool m_fullsreen = false;
    };

    namespace ImGuiWrap
    {
        class ImGuiLayer;
    }

    struct SGCORE_EXPORT Window
    {
        friend class ImGuiWrap::ImGuiLayer;
        
        static inline Signal<void(Window&)> onClose;
        static inline Signal<void(Window&, const int&)> onIconify;
        static inline Signal<void(Window&, const int&, const int&)> onFrameBufferSizeChanged;

#if SG_PLATFORM_PC
        using window_handle = GLFWwindow*;
#elif SG_PLATFORM_OS_ANDROID
        using window_handle = std::atomic<ANativeWindow*>;
#endif

#if SG_PLATFORM_OS_ANDROID
        friend void __AndroidImpl::setAndroidMainWindowHandle(ANativeWindow* window) noexcept;
#endif

        Window() noexcept = default;

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        /*Window(const Window& other) noexcept : m_config(other.m_config) { }

        explicit Window(WindowConfig&& otherConfig) noexcept : m_config(std::move(otherConfig)) { }*/

        ~Window() noexcept
        {
            destroy();
        }

        void create();
        void recreate();
        void destroy();

        void swapBuffers();
        void pollEvents();

        void makeCurrent() noexcept;

        void setSize(const int& sizeX, const int& sizeY) noexcept;

        void setSizeLimits(const int& sizeMinLimitX, const int& sizeMinLimitY, const int& sizeMaxLimitX, const int& sizeMaxLimitY) noexcept;

        void setPosition(const int& posX, const int& posY) noexcept;

        void setTitle(const std::string&) noexcept;

        void setSwapInterval(const bool&) noexcept;

        void setEnableStickyKeys(const bool&) noexcept;

        void setHideAndCentralizeCursor(const bool&) noexcept;
        bool isHideAndCentralizeCursor() noexcept;

        // -----------------

        void setShouldClose(const bool&) noexcept;

        void setShouldRecreate(bool value) noexcept;
        
        void setFullscreen(bool fullscreen) noexcept;
        bool isFullscreen() const noexcept;

        void setConfig(WindowConfig&& config) noexcept;
        
        bool shouldClose() noexcept;

        WindowConfig& getConfig() noexcept;

        void getSize(int& sizeX, int& sizeY) noexcept;

        void getBordersSize(int& left, int& top, int& right, int& bottom) noexcept;

        void getSizeWithBorders(int& sizeX, int& sizeY) noexcept;

        [[nodiscard]] double getSwapBuffersExecutionTime() const noexcept;

        window_handle getNativeHandle() noexcept;
        
        static void getPrimaryMonitorSize(int& sizeX, int& sizeY) noexcept;
        
        static int getPrimaryMonitorRefreshRate() noexcept;

    private:
        WindowConfig m_config;

        std::atomic<bool> m_shouldRecreate = false;
        
        double m_swapBuffersExecutionTime = 0.0;

        static void nativeCloseCallback(window_handle window) noexcept;
        static void nativeIconifyCallback(window_handle window, int iconified) noexcept;
        static void nativeFramebufferSizeCallback(window_handle window, int width, int height) noexcept;

        static void errorCallback(int errCode, const char* err_msg);

        window_handle m_handle = nullptr;

#if SG_PLATFORM_OS_ANDROID
        EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;
        EGLSurface m_eglSurface = EGL_NO_SURFACE;
        EGLContext m_eglContext = EGL_NO_CONTEXT;
        EGLConfig m_eglConfig = nullptr;
#endif
    };
}


#endif // SUNGEARENGINE_PROGRAMWINDOW_H
