//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_WINDOW_H
#define NATIVECORE_WINDOW_H

#include <iostream>
#include <utility>

#include "../Input/Keyboard.h"
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
        GLFWwindow* wnd;

        WindowConfig wnd_config;

        static void windowCloseCallback(GLFWwindow* window)
        {
            sgCallWindowCloseCallback(window);

            Logging::consolePrintf(Logging::MessageType::SG_INFO, "GLFW window closed.");
        }

        static void windowIconifyCallback(GLFWwindow* window, int iconified)
        {
            sgCallWindowIconifyCallback(window, iconified);

            Logging::consolePrintf(Logging::MessageType::SG_INFO, "GLFW window iconified.");
        }

        static void keyCallback(GLFWwindow* wnd, int key, int scanCode, int action, int mods)
        {
            Keyboard::keyCallback(wnd, key, scanCode, action, mods);

            sgCallWindowKeyCallback(wnd, key, scanCode, action, mods);
        }

        static void errorCallback(int err_code, const char* err_msg)
        {
            Logging::consolePrintf(Logging::SG_ERROR, "GLFW error (code %i): %s", err_code, err_msg);
        }

    public:
        Window() noexcept = default;

        Window(const Window& other) noexcept : wnd_config(other.wnd_config) { }

        Window(WindowConfig otherWindowConfig) noexcept : wnd_config(std::move(otherWindowConfig)) { }

        ~Window() noexcept { glfwDestroyWindow(wnd); }

        void create();

        void proceedFrame();

        void setSize(const int& sizeX, const int& sizeY) noexcept
        {
            wnd_config.size_x = sizeX;
            wnd_config.size_y = sizeY;

            glfwSetWindowSize(wnd, sizeX, sizeY);
        }

        void setPosition(const int& posX, const int& posY) noexcept
        {
            wnd_config.positionX = posX;
            wnd_config.positionY = posY;

            glfwSetWindowSize(wnd, posX, posY);
        }

        void setSizeLimits(const int& sizeMinLimitX, const int& sizeMinLimitY, const int& sizeMaxLimitX, const int& sizeMaxLimitY) noexcept
        {
            wnd_config.sizeMinLimitX = sizeMinLimitX;
            wnd_config.sizeMinLimitY = sizeMinLimitY;

            wnd_config.sizeMaxLimitX = sizeMaxLimitX;
            wnd_config.sizeMaxLimitY = sizeMaxLimitY;

            glfwSetWindowSizeLimits(wnd, sizeMinLimitX, sizeMinLimitY, sizeMaxLimitX, sizeMaxLimitY);
        }

        void setSwapInterval(const bool& swapInterval) noexcept
        {
            wnd_config.swapInterval = swapInterval;

            glfwSwapInterval(swapInterval);
        }

        void setEnableStickyKeys(const bool& enableStickyKeys) noexcept
        {
            wnd_config.enableStickyKeys = enableStickyKeys;

            glfwSetInputMode(wnd, GLFW_STICKY_KEYS, enableStickyKeys);
        }

        inline void setConfig(const WindowConfig& other) noexcept
        {
            wnd_config = other;
        }

        inline bool shouldClose() noexcept
        {
            return glfwWindowShouldClose(wnd);
        }

        inline void setShouldClose(bool shouldClose) noexcept
        {
            glfwSetWindowShouldClose(wnd, shouldClose);
        }

        inline void makeCurrent() noexcept
        {
            glfwMakeContextCurrent(wnd);
        }

        inline GLFWwindow* getHandler() noexcept
        {
            return wnd;
        }

        inline WindowConfig getConfig() noexcept
        {
            return wnd_config;
        }

        inline void getSize(int& sizeX, int& sizeY) noexcept
        {
            glfwGetWindowSize(wnd, &sizeX, &sizeY);
        }

        static inline void getPrimaryMonitorSize(int& sizeX, int& sizeY) noexcept
        {
            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            glfwGetMonitorPhysicalSize(primaryMonitor, &sizeX, &sizeY);
        }
    };
}


#endif //NATIVECORE_WINDOW_H
