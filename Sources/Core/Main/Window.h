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
        int m_sizeX = 500;
        int m_sizeY = 250;

        int m_sizeMinLimitX = -1;
        int m_sizeMinLimitY = -1;

        int m_sizeMaxLimitX = -1;
        int m_sizeMaxLimitY = -1;

        int m_positionX = 100;
        int m_positionY = 100;

        std::string m_title = "Powered by Core";

        bool m_swapInterval = true;

        bool m_enableStickyKeys = false;

        explicit WindowConfig() noexcept = default;
    };

    class Window
    {
    private:
        GLFWwindow* m_handler;

        WindowConfig m_config;

        static void windowCloseCallback(GLFWwindow* window);

        static void windowIconifyCallback(GLFWwindow* window, int iconified);

        static void errorCallback(int errCode, const char* err_msg);

    public:
        Window() noexcept = default;

        Window(const Window& other) noexcept : m_config(other.m_config) { }

        explicit Window(WindowConfig otherConfig) noexcept : m_config(std::move(otherConfig)) { }

        ~Window() noexcept { glfwDestroyWindow(m_handler); }

        void create();

        void proceedFrame();

        void makeCurrent() noexcept;

        #pragma region Setters

        void setSize(const int& sizeX, const int& sizeY) noexcept;

        void setPosition(const int& posX, const int& posY) noexcept;

        void setSizeLimits(const int& sizeMinLimitX, const int& sizeMinLimitY, const int& sizeMaxLimitX, const int& sizeMaxLimitY) noexcept;

        void setSwapInterval(const bool&) noexcept;

        void setEnableStickyKeys(const bool&) noexcept;

        void setConfig(const WindowConfig&) noexcept;

        void setShouldClose(const bool&) noexcept;

        void setTitle(const std::string&) noexcept;

        #pragma endregion

        #pragma region Getters

        bool shouldClose() noexcept;

        const WindowConfig& getConfig() noexcept;

        void getSize(int& sizeX, int& sizeY) noexcept;

        static void getPrimaryMonitorSize(int& sizeX, int& sizeY) noexcept;

        #pragma endregion
    };
}


#endif //NATIVECORE_WINDOW_H
