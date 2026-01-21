//
// Created by stuka on 21.01.2026.
//

#pragma once

namespace SGCore::GOAP
{
    /**
     * Action or plan execution result.
     */
    enum class ExecutionResult
    {
        EXEC_SUCCESS,
        EXEC_PAUSED,
        EXEC_FAILED
    };
}