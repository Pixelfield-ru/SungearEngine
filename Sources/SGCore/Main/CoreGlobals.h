//
// Created by stuka on 20.11.2023.
//

#ifndef SUNGEARENGINE_COREGLOBALS_H
#define SUNGEARENGINE_COREGLOBALS_H

#include <memory>

namespace SGCore
{
    enum class highp_entity : std::uint64_t { };
    
    using entity_t = highp_entity;
    
    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename... Args>
    constexpr Ref<T> MakeRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename... Args>
    constexpr Scope<T> MakeScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Weak = std::weak_ptr<T>;
}

#endif //SUNGEARENGINE_COREGLOBALS_H
