//
// Created by stuka on 13.01.2025.
//

#pragma once

#include <sgcore_export.h>

namespace SGCore
{
    struct ICommand
    {
        SGCORE_EXPORT virtual ~ICommand() = default;

        virtual void execute() = 0;
        virtual void undo() = 0;
    };
}
