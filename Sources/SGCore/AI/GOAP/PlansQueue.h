//
// Created by stuka on 18.01.2026.
//

#pragma once

#include <queue>

#include "Goal.h"

namespace SGCore::GOAP
{
    struct PlansQueue
    {
        // void_sex addPlan(std::int32_t priority)

    private:
        std::queue<Goal> m_queue;
    };
}
