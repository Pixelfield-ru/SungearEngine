//
// Created by stuka on 30.10.2023.
//

#include "ISystem.h"

double SGCore::ISystem::getUpdateFunctionExecutionTime() const noexcept
{
    return m_update_executionTime;
}

double SGCore::ISystem::getFixedUpdateFunctionExecutionTime() const noexcept
{
    return m_fixedUpdate_executionTime;
}
