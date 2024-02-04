//
// Created by stuka on 05.02.2024.
//

#ifndef SUNGEARENGINE_GPUOBJECT_H
#define SUNGEARENGINE_GPUOBJECT_H

namespace SGCore
{
    struct GPUObject
    {
        virtual void addToGlobalStorage() noexcept { }
    };
}

#endif //SUNGEARENGINE_GPUOBJECT_H
