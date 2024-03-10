//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_IPARALLELSYSTEMSUBPROCESS_H
#define SUNGEARENGINE_IPARALLELSYSTEMSUBPROCESS_H

namespace SGCore
{
    template<typename ParentParallelSystemT>
    struct IParallelSystemSubprocess
    {
        virtual void parallelUpdate(const double& dt, const double& fixedDt, Ref<ParentParallelSystemT> parentParallelSystem) noexcept { }
    };
}

#endif //SUNGEARENGINE_IPARALLELSYSTEMSUBPROCESS_H
