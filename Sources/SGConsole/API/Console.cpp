#include "Console.h"

#include <iostream>
#include <string_view>
//#include <fstream>
#include <ranges>

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#include <cstdio>
#include <fcntl.h>
#include <io.h>
#include <unistd.h>
#include <charconv>

// TODO: make errors catch
void SGConsole::Console::start() noexcept
{
    SHELLEXECUTEINFO shExecInfo;

    shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

    shExecInfo.fMask = NULL;
    shExecInfo.hwnd = NULL;
    shExecInfo.lpVerb = "runas";
    shExecInfo.lpFile = "SGConsoleApp.exe";
    shExecInfo.lpParameters = "SGConsoleAPId.dll";
    shExecInfo.lpDirectory = NULL;
    shExecInfo.nShow = SW_NORMAL;
    shExecInfo.hInstApp = NULL;

    m_procID  = ShellExecuteEx(&shExecInfo);

    m_running = true;
}

#elif __linux__

void SGConsole::Console::start() noexcept
{

}

#elif defined(__APPLE__)

#include <TargetConditionals.h>

void SGConsole::Console::start() noexcept
{

}

#endif

void SGConsole::Console::stop() noexcept
{
    m_running = false;
}

bool SGConsole::Console::isRunning() noexcept
{
    return m_running;
}

void SGConsole::Console::sendCommand(const std::string& command) noexcept
{
    m_currentCommandInVector = 0;

    std::regex commandRegex("[ ]");
    std::sregex_token_iterator first{command.begin(), command.end(), commandRegex, -1}, last; //the '-1' is what makes the regex split (-1 := what was not matched)

    std::vector<std::string> commandWords { first, last };
    m_commands.emplace_back(commandWords);

    for(int i = 0; i < commandWords.size(); i++)
    {
        std::cout << commandWords[i] << std::endl;
    }
}
