//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_WINDOW_H
#define NATIVECORE_WINDOW_H

#include <iostream>
#include <utility>

#include "../Input/InputManager.h"
#include "../Memory/Resource.h"
#include "../Logging/Log.h"

#include "Callbacks.h"

namespace Core::Main
{
    struct WindowConfig
    {
        int size_x = 500;
        int size_y = 250;

        int sizeMinLimitX = -1;
        int sizeMinLimitY = -1;

        int sizeMaxLimitX = -1;
        int sizeMaxLimitY = -1;

        int positionX = 100;
        int positionY = 100;

        std::string name = "Powered by Core";

        bool swapInterval = true;

        bool enableStickyKeys = false;

        explicit WindowConfig() noexcept = default;
    };

    class Window
    {
    private:
        GLFWwindow* handler;

        WindowConfig wndConfig;

        static void windowCloseCallback(GLFWwindow* window);

        static void windowIconifyCallback(GLFWwindow* window, int iconified);

        static void errorCallback(int errCode, const char* err_msg);

    public:
        Window() noexcept = default;

        Window(const Window& other) noexcept : wndConfig(other.wndConfig) { }

        Window(WindowConfig otherWindowConfig) noexcept : wndConfig(std::move(otherWindowConfig)) { }

        ~Window() noexcept { glfwDestroyWindow(handler); }

        void create();

        void proceedFrame();

        void setSize(const int& sizeX, const int& sizeY) noexcept;

        void setPosition(const int& posX, const int& posY) noexcept;

        void setSizeLimits(const int& sizeMinLimitX, const int& sizeMinLimitY, const int& sizeMaxLimitX, const int& sizeMaxLimitY) noexcept;

        void setSwapInterval(const bool& swapInterval) noexcept;

        void setEnableStickyKeys(const bool& enableStickyKeys) noexcept;

        inline void setConfig(const WindowConfig& other) noexcept
        {
            wndConfig = other;
        }

        inline bool shouldClose() noexcept
        {
            return glfwWindowShouldClose(handler);
        }

        inline void setShouldClose(bool shouldClose) noexcept
        {
            glfwSetWindowShouldClose(handler, shouldClose);
        }

        inline void makeCurrent() noexcept
        {
            glfwMakeContextCurrent(handler);
        }

        inline const WindowConfig& getConfig() noexcept
        {
            return wndConfig;
        }

        inline void getSize(int& sizeX, int& sizeY) noexcept
        {
            glfwGetWindowSize(handler, &sizeX, &sizeY);
        }

        static inline void getPrimaryMonitorSize(int& sizeX, int& sizeY) noexcept;
    };
}


#endif //NATIVECORE_WINDOW_H
