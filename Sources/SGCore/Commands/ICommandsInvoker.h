//
// Created by stuka on 13.01.2025.
//

#ifndef SUNGEARENGINE_ICOMMANDSINVOKER_H
#define SUNGEARENGINE_ICOMMANDSINVOKER_H

#include <stack>
#include <memory>

#include "ICommand.h"

namespace SGCore
{
    struct ICommandsInvoker
    {
        virtual ~ICommandsInvoker() = default;

        virtual void pushCommand(const std::shared_ptr<ICommand>& command)
        {
            m_notExecutedCommands.push(command);
        }

        virtual void popCommand()
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

#endif //SUNGEARENGINE_ICOMMANDSINVOKER_H
