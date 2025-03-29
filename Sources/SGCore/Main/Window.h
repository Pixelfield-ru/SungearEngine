#ifndef SUNGEARENGINE_PROGRAMWINDOW_H
#define SUNGEARENGINE_PROGRAMWINDOW_H

#define GLFW_INCLUDE_NONE

#include <SGCore/pch.h>

#include <sgcore_export.h>

#include "SGCore/Utils/Signal.h"

namespace SGCore
{
    enum KeyState : std::int32_t
    {
        PRESS = GLFW_PRESS,
        RELEASE = GLFW_RELEASE,
        REPEAT = GLFW_REPEAT
    };
    
    enum class KeyboardKey : std::int32_t
    {
        KEY_FIRST           = GLFW_KEY_SPACE,
        KEY_SPACE           = GLFW_KEY_SPACE,
        KEY_APOSTROPHE      = GLFW_KEY_APOSTROPHE,
        KEY_COMMA           = GLFW_KEY_COMMA,
        KEY_MINUS           = GLFW_KEY_MINUS,
        KEY_PERIOD          = GLFW_KEY_PERIOD,
        KEY_SLASH           = GLFW_KEY_SLASH,
        KEY_0               = GLFW_KEY_0,
        KEY_1               = GLFW_KEY_1,
        KEY_2               = GLFW_KEY_2,
        KEY_3               = GLFW_KEY_3,
        KEY_4               = GLFW_KEY_4,
        KEY_5               = GLFW_KEY_5,
        KEY_6               = GLFW_KEY_6,
        KEY_7               = GLFW_KEY_7,
        KEY_8               = GLFW_KEY_8,
        KEY_9               = GLFW_KEY_9,
        KEY_SEMICOLON       = GLFW_KEY_SEMICOLON,
        KEY_EQUAL           = GLFW_KEY_EQUAL,
        KEY_A               = GLFW_KEY_A,
        KEY_B               = GLFW_KEY_B,
        KEY_C               = GLFW_KEY_C,
        KEY_D               = GLFW_KEY_D,
        KEY_E               = GLFW_KEY_E,
        KEY_F               = GLFW_KEY_F,
        KEY_G               = GLFW_KEY_G,
        KEY_H               = GLFW_KEY_H,
        KEY_I               = GLFW_KEY_I,
        KEY_J               = GLFW_KEY_J,
        KEY_K               = GLFW_KEY_K,
        KEY_L               = GLFW_KEY_L,
        KEY_M               = GLFW_KEY_M,
        KEY_N               = GLFW_KEY_N,
        KEY_O               = GLFW_KEY_O,
        KEY_P               = GLFW_KEY_P,
        KEY_Q               = GLFW_KEY_Q,
        KEY_R               = GLFW_KEY_R,
        KEY_S               = GLFW_KEY_S,
        KEY_T               = GLFW_KEY_T,
        KEY_U               = GLFW_KEY_U,
        KEY_V               = GLFW_KEY_V,
        KEY_W               = GLFW_KEY_W,
        KEY_X               = GLFW_KEY_X,
        KEY_Y               = GLFW_KEY_Y,
        KEY_Z               = GLFW_KEY_Z,
        KEY_LEFT_BRACKET    = GLFW_KEY_LEFT_BRACKET,
        KEY_BACKSLASH       = GLFW_KEY_BACKSLASH,
        KEY_RIGHT_BRACKET   = GLFW_KEY_RIGHT_BRACKET,
        KEY_GRAVE_ACCENT    = GLFW_KEY_GRAVE_ACCENT,
        KEY_WORLD_1         = GLFW_KEY_WORLD_1,
        KEY_WORLD_2         = GLFW_KEY_WORLD_2,
        KEY_ESCAPE          = GLFW_KEY_ESCAPE,
        KEY_ENTER           = GLFW_KEY_ENTER,
        KEY_TAB             = GLFW_KEY_TAB,
        KEY_BACKSPACE       = GLFW_KEY_BACKSPACE,
        KEY_INSERT          = GLFW_KEY_INSERT,
        KEY_DELETE          = GLFW_KEY_DELETE,
        KEY_RIGHT           = GLFW_KEY_RIGHT,
        KEY_LEFT            = GLFW_KEY_LEFT,
        KEY_DOWN            = GLFW_KEY_DOWN,
        KEY_UP              = GLFW_KEY_UP,
        KEY_PAGE_UP         = GLFW_KEY_PAGE_UP,
        KEY_PAGE_DOWN       = GLFW_KEY_PAGE_DOWN,
        KEY_HOME            = GLFW_KEY_HOME,
        KEY_END             = GLFW_KEY_END,
        KEY_CAPS_LOCK       = GLFW_KEY_CAPS_LOCK,
        KEY_SCROLL_LOCK     = GLFW_KEY_SCROLL_LOCK,
        KEY_NUM_LOCK        = GLFW_KEY_NUM_LOCK,
        KEY_PRINT_SCREEN    = GLFW_KEY_PRINT_SCREEN,
        KEY_PAUSE           = GLFW_KEY_PAUSE,
        KEY_F1              = GLFW_KEY_F1,
        KEY_F2              = GLFW_KEY_F2,
        KEY_F3              = GLFW_KEY_F3,
        KEY_F4              = GLFW_KEY_F4,
        KEY_F5              = GLFW_KEY_F5,
        KEY_F6              = GLFW_KEY_F6,
        KEY_F7              = GLFW_KEY_F7,
        KEY_F8              = GLFW_KEY_F8,
        KEY_F9              = GLFW_KEY_F9,
        KEY_F10             = GLFW_KEY_F10,
        KEY_F11             = GLFW_KEY_F11,
        KEY_F12             = GLFW_KEY_F12,
        KEY_F13             = GLFW_KEY_F13,
        KEY_F14             = GLFW_KEY_F14,
        KEY_F15             = GLFW_KEY_F15,
        KEY_F16             = GLFW_KEY_F16,
        KEY_F17             = GLFW_KEY_F17,
        KEY_F18             = GLFW_KEY_F18,
        KEY_F19             = GLFW_KEY_F19,
        KEY_F20             = GLFW_KEY_F20,
        KEY_F21             = GLFW_KEY_F21,
        KEY_F22             = GLFW_KEY_F22,
        KEY_F23             = GLFW_KEY_F23,
        KEY_F24             = GLFW_KEY_F24,
        KEY_F25             = GLFW_KEY_F25,
        KEY_KP_0            = GLFW_KEY_KP_0,
        KEY_KP_1            = GLFW_KEY_KP_1,
        KEY_KP_2            = GLFW_KEY_KP_2,
        KEY_KP_3            = GLFW_KEY_KP_3,
        KEY_KP_4            = GLFW_KEY_KP_4,
        KEY_KP_5            = GLFW_KEY_KP_5,
        KEY_KP_6            = GLFW_KEY_KP_6,
        KEY_KP_7            = GLFW_KEY_KP_7,
        KEY_KP_8            = GLFW_KEY_KP_8,
        KEY_KP_9            = GLFW_KEY_KP_9,
        KEY_KP_DECIMAL      = GLFW_KEY_KP_DECIMAL,
        KEY_KP_DIVIDE       = GLFW_KEY_KP_DIVIDE,
        KEY_KP_MULTIPLY     = GLFW_KEY_KP_MULTIPLY,
        KEY_KP_SUBTRACT     = GLFW_KEY_KP_SUBTRACT,
        KEY_KP_ADD          = GLFW_KEY_KP_ADD,
        KEY_KP_ENTER        = GLFW_KEY_KP_ENTER,
        KEY_KP_EQUAL        = GLFW_KEY_KP_EQUAL,
        KEY_LEFT_SHIFT      = GLFW_KEY_LEFT_SHIFT,
        KEY_LEFT_CONTROL    = GLFW_KEY_LEFT_CONTROL,
        KEY_LEFT_ALT        = GLFW_KEY_LEFT_ALT,
        KEY_LEFT_SUPER      = GLFW_KEY_LEFT_SUPER,
        KEY_RIGHT_SHIFT     = GLFW_KEY_RIGHT_SHIFT,
        KEY_RIGHT_CONTROL   = GLFW_KEY_RIGHT_CONTROL,
        KEY_RIGHT_ALT       = GLFW_KEY_RIGHT_ALT,
        KEY_RIGHT_SUPER     = GLFW_KEY_RIGHT_SUPER,
        KEY_MENU            = GLFW_KEY_MENU,
        KEY_LAST            = GLFW_KEY_LAST
    };
    
    enum class MouseButton : std::int32_t
    {
        MOUSE_BUTTON_1 = GLFW_MOUSE_BUTTON_1,
        MOUSE_BUTTON_2 = GLFW_MOUSE_BUTTON_2,
        MOUSE_BUTTON_3 = GLFW_MOUSE_BUTTON_3,
        MOUSE_BUTTON_4 = GLFW_MOUSE_BUTTON_4,
        MOUSE_BUTTON_5 = GLFW_MOUSE_BUTTON_5,
        MOUSE_BUTTON_6 = GLFW_MOUSE_BUTTON_6,
        MOUSE_BUTTON_7 = GLFW_MOUSE_BUTTON_7,
        MOUSE_BUTTON_8 = GLFW_MOUSE_BUTTON_8,
        MOUSE_BUTTON_FIRST = MOUSE_BUTTON_1,
        MOUSE_BUTTON_LAST = MOUSE_BUTTON_8,
        MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1,
        MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2,
        MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3
    };
    
    struct Window;

    /**
     * Note that the config changes dynamically.
     */
     // TODO: make config save
    class WindowConfig
    {
        friend class Window;
        
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
        
        static inline Signal<void(Window&, const KeyboardKey&, const int&, const KeyState&, const int&)> onKeyboardKeyEvent;
        
        static inline Signal<void(Window&, const MouseButton&, const KeyState&, const int&)> onMouseButtonEvent;
        
        static inline Signal<void(Window&, const double&, const double&)> onCursorPositionChanged;

        static inline Signal<void(Window&, const int&, const int&)> onFrameBufferSizeChanged;

        Window() noexcept = default;

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        /*Window(const Window& other) noexcept : m_config(other.m_config) { }

        explicit Window(WindowConfig&& otherConfig) noexcept : m_config(std::move(otherConfig)) { }*/

        ~Window() noexcept
        {
            glfwMakeContextCurrent(nullptr);
            glfwSetWindowShouldClose(m_handler, GLFW_TRUE);
            // glfwDestroyWindow(m_handler);
            m_handler = nullptr;
        }

        void create();
        void recreate();

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

        void setCursorPosition(const double&, const double&) noexcept;
        void getCursorPosition(double& posX, double& posY) noexcept;
        
        KeyState getKeyboardKeyState(const KeyboardKey& key);
        KeyState getMouseButtonState(const MouseButton& button);
        
        void setFullscreen(bool fullscreen) noexcept;
        bool isFullscreen() const noexcept;

        void setConfig(WindowConfig&& config) noexcept;
        
        bool shouldClose() noexcept;

        WindowConfig& getConfig() noexcept;

        void getSize(int& sizeX, int& sizeY) noexcept;

        [[nodiscard]] double getSwapBuffersExecutionTime() const noexcept;
        
        static void getPrimaryMonitorSize(int& sizeX, int& sizeY) noexcept;
        
        static int getPrimaryMonitorRefreshRate() noexcept;

    private:
        WindowConfig m_config;
        
        double m_swapBuffersExecutionTime = 0.0;

        static void nativeCloseCallback(GLFWwindow* window) noexcept;
        static void nativeIconifyCallback(GLFWwindow* window, int iconified) noexcept;
        static void nativeKeyboardKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;
        static void nativeMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) noexcept;
        static void nativeMousePositionCallback(GLFWwindow* window, double xpos, double ypos) noexcept;
        static void nativeFramebufferSizeCallback(GLFWwindow* window, int width, int height) noexcept;

        static void errorCallback(int errCode, const char* err_msg);

        GLFWwindow* m_handler = nullptr;
    };
}


#endif // SUNGEARENGINE_PROGRAMWINDOW_H
