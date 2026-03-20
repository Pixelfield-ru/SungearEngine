//
// Created by stuka on 13.01.2025.
//

#pragma once

#include <stack>
#include <memory>

#include "ICommand.h"

namespace SGCore
{
    struct ICommandsInvoker
    {
        SGCORE_EXPORT virtual ~ICommandsInvoker() = default;

        SGCORE_EXPORT virtual void pushCommand(const std::shared_ptr<ICommand>& command)
        {
            m_notExecutedCommands.push(command);
        }

        SGCORE_EXPORT virtual void popCommand()
        {
            m_notExecutedCommands.pop();
        }

        virtual void executeCommand() = 0;

        virtual void executeAllCommands() = 0;

        virtual void undoCommand() = 0;

        virtual void undoAllCommands() = 0;

    protected:
        std::stack<std::shared_ptr<ICommand>> m_notExecutedCommands;
        std::stack<std::shared_ptr<ICommand>> m_executedCommands;
    };
}
