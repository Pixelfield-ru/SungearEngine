//
// Created by stuka on 20.11.2023.
//

#ifndef SUNGEARENGINE_COREGLOBALS_H
#define SUNGEARENGINE_COREGLOBALS_H

#include <SGCore/pch.h>
// #include "SGCore/ECS/ECSRegistry.h"

namespace SGCore
{
    namespace ECS
    {
        template<typename>
        struct Registry;

        enum class highp_entity : std::uint64_t { };

        using entity_t = highp_entity;

        using registry_t = ECS::Registry<entity_t>;
    }

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
    
    // ================================ GLM USINGS
    
    using uivec2_32 = glm::vec<2, std::uint32_t, glm::defaultp>;
    using uivec3_32 = glm::vec<3, std::uint32_t, glm::defaultp>;
    using uivec4_32 = glm::vec<4, std::uint32_t, glm::defaultp>;
    
    using uivec2_64 = glm::vec<2, std::uint64_t, glm::defaultp>;
    using uivec3_64 = glm::vec<3, std::uint64_t, glm::defaultp>;
    using uivec4_64 = glm::vec<4, std::uint64_t, glm::defaultp>;
    
    using ivec2_32 = glm::vec<2, std::int32_t, glm::defaultp>;
    using ivec3_32 = glm::vec<3, std::int32_t, glm::defaultp>;
    using ivec4_32 = glm::vec<4, std::int32_t, glm::defaultp>;
    
    using ivec2_64 = glm::vec<2, std::int64_t, glm::defaultp>;
    using ivec3_64 = glm::vec<3, std::int64_t, glm::defaultp>;
    using ivec4_64 = glm::vec<4, std::int64_t, glm::defaultp>;
    
    // ===========================================
}

#endif //SUNGEARENGINE_COREGLOBALS_H
