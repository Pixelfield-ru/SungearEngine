//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_WINDOW_H
#define NATIVECORE_WINDOW_H

#include <iostream>
#include <utility>

#include "Callbacks.h"

namespace SGCore
{
    class Window;

    /**
     * Note that the config changes dynamically.
     */
     // TODO: make config save
    class WindowConfig
    {
        friend class Window;

    private:
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

    public:
        WindowConfig() noexcept = default;
    };

    namespace ImGuiWrap
    {
        class ImGuiLayer;
    }

    class Window
    {
        friend class ImGuiWrap::ImGuiLayer;

    public:
        Window() noexcept = default;

        Window(const Window& other) noexcept : m_config(other.m_config) { }

        explicit Window(WindowConfig&& otherConfig) noexcept : m_config(std::move(otherConfig)) { }

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

        void setFullscreen(bool fullscreen) noexcept;
        bool isFullscreen() const noexcept;

        void setConfig(WindowConfig&& config) noexcept;
        
        bool shouldClose() noexcept;

        WindowConfig& getConfig() noexcept;

        void getSize(int& sizeX, int& sizeY) noexcept;

        [[nodiscard]] double getSwapBuffersExecutionTime() const noexcept;
        
        static void getPrimaryMonitorSize(int& sizeX, int& sizeY) noexcept;

    private:
        WindowConfig m_config;
        
        double m_swapBuffersExecutionTime = 0.0;

        static void windowCloseCallback(GLFWwindow* window);

        static void windowIconifyCallback(GLFWwindow* window, int iconified);

        static void errorCallback(int errCode, const char* err_msg);

        GLFWwindow* m_handler = nullptr;
    };
}


#endif //NATIVECORE_WINDOW_H
