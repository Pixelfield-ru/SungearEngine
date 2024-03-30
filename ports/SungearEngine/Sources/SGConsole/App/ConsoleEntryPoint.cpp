//
// Created by stuka on 03.07.2023.
//

#include "ConsoleEntryPoint.h"

#include <iostream>

#include "SGConsole/API/Console.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"

int main()
{
    std::string command;

    while(true)
    {
        std::cout << "SGConsole -> ";
        std::getline(std::cin, command);

        SGConsole::Console::sendCommand(command);
    }

    return 0;
}
