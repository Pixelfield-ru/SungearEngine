//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_WINDOW_H
#define NATIVECORE_WINDOW_H

#include <iostream>
#include <utility>

#include "SGCore/Input/InputManager.h"
#include "SGCore/Memory/Resource.h"
#include "SGCore/Logging/Log.h"

#include "Callbacks.h"

namespace Core::Main
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

    public:
        WindowConfig() noexcept = default;
    };

    class Window
    {
    private:
        GLFWwindow* m_handler = nullptr;

        std::shared_ptr<WindowConfig> m_config = std::make_shared<WindowConfig>();

        static void windowCloseCallback(GLFWwindow* window);

        static void windowIconifyCallback(GLFWwindow* window, int iconified);

        static void errorCallback(int errCode, const char* err_msg);

    public:
        Window() noexcept = default;

        Window(const Window& other) noexcept : m_config(other.m_config) { }

        explicit Window(std::shared_ptr<WindowConfig> otherConfig) noexcept : m_config(std::move(otherConfig)) { }

        ~Window() noexcept
        {
            glfwMakeContextCurrent(nullptr);
            glfwDestroyWindow(m_handler);
        }

        void create();

        void swapBuffers();
        void pollEvents();

        void makeCurrent() noexcept;

        #pragma region Setters

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

        void setConfig(const std::shared_ptr<WindowConfig>&) noexcept;

        #pragma endregion

        #pragma region Getters

        bool shouldClose() noexcept;

        std::shared_ptr<WindowConfig> getConfig() noexcept;

        void getSize(int& sizeX, int& sizeY) noexcept;

        static void getPrimaryMonitorSize(int& sizeX, int& sizeY) noexcept;

        #pragma endregion
    };
}


#endif //NATIVECORE_WINDOW_H
