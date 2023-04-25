//
// Created by stuka on 17.04.2023.
//

#pragma once

#ifndef NATIVECORE_CORE_H
#define NATIVECORE_CORE_H

namespace Core::Main
{
    class Core
    {
    private:
        inline static Window window{};
        inline static Graphics::API::IRenderer* renderer;
    public:
        Core() = delete;

        static void start();

        static inline Window& get_window() noexcept
        {
            return window;
        }

        static inline Graphics::API::IRenderer& get_renderer() noexcept
        {
            return *renderer;
        }
    };
}


#endif //NATIVECORE_CORE_H
