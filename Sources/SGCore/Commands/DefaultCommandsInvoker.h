//
// Created by stuka on 13.01.2025.
//

#ifndef SUNGEARENGINE_DEFAULTCOMMANDSINVOKER_H
#define SUNGEARENGINE_DEFAULTCOMMANDSINVOKER_H

#include "ICommandsInvoker.h"

namespace SGCore
{
    struct DefaultCommandsInvoker : ICommandsInvoker
    {
        void executeCommand() override
        {
            auto command = m_notExecutedCommands.top();
            m_notExecutedCommands.pop();

            command->execute();

            m_executedCommands.push(command);
        }

        void executeAllCommands() override
        {
            size_t commandsCount = m_notExecutedCommands.size();
            for(size_t i = 0; i < commandsCount; ++i)
            {
                executeCommand();
            }
        }

        void undoCommand() override
        {
            auto command = m_executedCommands.top();
            m_executedCommands.pop();

            command->undo();

            m_notExecutedCommands.push(command);
        }

        void undoAllCommands() override
        {
            size_t commandsCount = m_executedCommands.size();

            for(size_t i = 0; i < commandsCount; ++i)
            {
                undoCommand();
            }
        }
    };
}

#endif //SUNGEARENGINE_DEFAULTCOMMANDSINVOKER_H
