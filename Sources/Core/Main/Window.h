//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_WINDOW_H
#define NATIVECORE_WINDOW_H

#include <iostream>
#include <utility>

#include "../Memory/Resource.h"
#include "../Logging/Log.h"
#include <GLFW/glfw3.h>

#include "Callbacks.h"

namespace Core::Main
{
    struct WindowConfig
    {
        int size_x = 500;
        int size_y = 250;

        int size_min_limit_x = 0;
        int size_min_limit_y = 0;

        int size_max_limit_x = 0;
        int size_max_limit_y = 0;

        int position_x = 100;
        int position_y = 100;

        std::string name = "Powered by Core";

        bool swap_interval = true;

        explicit WindowConfig() noexcept = default;
    };

    class Window
    {
    private:
        GLFWwindow* wnd;

        WindowConfig wnd_config;

        static void window_close_callback(GLFWwindow* window)
        {
            sg_call_window_close_callback(window);

            Logging::c_printf(Logging::MessageType::SG_INFO, "GLFW window closed.");
        }

        static void window_iconify_callback(GLFWwindow* window, int iconified)
        {
            sg_call_window_iconify_callback(window, iconified);

            Logging::c_printf(Logging::MessageType::SG_INFO, "GLFW window iconified.");
        }

        static void error_callback(int err_code, const char* err_msg)
        {
            Logging::c_printf(Logging::SG_ERROR, "GLFW error (code %i): %s", err_code, err_msg);
        }

    public:
        Window() noexcept = default;

        Window(const Window& other) noexcept : wnd_config(other.wnd_config) { }

        Window(const WindowConfig& other_window_config) noexcept : wnd_config(other_window_config) { }

        ~Window() noexcept { glfwDestroyWindow(wnd); }

        void create();

        void proceed_frame();

        void set_size(const int& sz_x, const int& sz_y) noexcept
        {
            wnd_config.size_x = sz_x;
            wnd_config.size_y = sz_y;

            glfwSetWindowSize(wnd, sz_x, sz_y);
        }

        void set_position(const int& pos_x, const int& pos_y) noexcept
        {
            wnd_config.position_x = pos_x;
            wnd_config.position_y = pos_y;

            glfwSetWindowSize(wnd, pos_x, pos_y);
        }

        void set_swap_interval(const bool& swap_interval) noexcept
        {
            wnd_config.swap_interval = swap_interval;

            glfwSwapInterval(swap_interval);
        }

        void set_size_limits(const int& size_min_limit_x, const int& size_min_limit_y, const int& size_max_limit_x, const int& size_max_limit_y)
        {
            wnd_config.size_min_limit_x = size_min_limit_x;
            wnd_config.size_min_limit_y = size_min_limit_y;

            wnd_config.size_max_limit_x = size_max_limit_x;
            wnd_config.size_max_limit_y = size_max_limit_y;

            glfwSetWindowSizeLimits(wnd, size_min_limit_x, size_min_limit_y, size_max_limit_x, size_max_limit_y);
        }

        inline void set_config(const WindowConfig& other) noexcept
        {
            wnd_config = other;
        }

        inline bool should_close() noexcept
        {
            return glfwWindowShouldClose(wnd);
        }

        inline void set_should_close(bool should_close) noexcept
        {
            glfwSetWindowShouldClose(wnd, should_close);
        }
    };
}


#endif //NATIVECORE_WINDOW_H
