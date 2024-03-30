#pragma once

#ifndef SUNGEARENGINE_CONSOLE_H
#define SUNGEARENGINE_CONSOLE_H

#include <vector>
#include <string>
#include <regex>

namespace SGConsole
{
    class Console
    {
    private:
        static inline int m_procID = 0;
        static inline bool m_running = false;

        static inline int m_currentCommandInVector= 0;

        // command - word
        static inline std::vector<std::vector<std::string>> m_commands;

    public:
        static void start() noexcept;
        static void stop() noexcept;

        static bool isRunning() noexcept;

        static void sendCommand(const std::string& command) noexcept;
    };
}

#endif //SUNGEARENGINE_CONSOLE_H
