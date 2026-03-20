//
// Created by ilya on 10.03.24.
//

#pragma once

namespace SGCore
{
    template<typename ParentParallelSystemT>
    struct IParallelSystemSubprocess
    {
        virtual ~IParallelSystemSubprocess() = default;

        virtual void parallelUpdate(const double& dt, const double& fixedDt, ParentParallelSystemT* parentParallelSystem) noexcept { }
        virtual void fixedUpdate(const double& dt, const double& fixedDt, ParentParallelSystemT* parentParallelSystem) noexcept { }
    };
}
