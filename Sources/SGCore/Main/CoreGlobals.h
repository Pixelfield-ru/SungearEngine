//
// Created by stuka on 20.11.2023.
//

#ifndef SUNGEARENGINE_COREGLOBALS_H
#define SUNGEARENGINE_COREGLOBALS_H

#include <memory>

namespace SGCore
{
    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
    using Scope = std::unique_ptr<T>;
}

#endif //SUNGEARENGINE_COREGLOBALS_H
