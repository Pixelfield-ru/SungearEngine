//
// Created by stuka on 30.12.2024.
//

#ifndef SUNGEARENGINE_ASSETSLOADPOLICY_H
#define SUNGEARENGINE_ASSETSLOADPOLICY_H

namespace SGCore
{
    enum AssetsLoadPolicy
    {
        SINGLE_THREADED,
        PARALLEL_THEN_LAZYLOAD,
        PARALLEL_NO_LAZYLOAD
    };
}

#endif //SUNGEARENGINE_ASSETSLOADPOLICY_H
