//
// Created by stuka on 13.01.2025.
//

#ifndef SUNGEARENGINE_ICOMMAND_H
#define SUNGEARENGINE_ICOMMAND_H

namespace SGCore
{
    struct ICommand
    {
        virtual void execute() = 0;
        virtual void undo() = 0;
    };
}

#endif //SUNGEARENGINE_ICOMMAND_H
