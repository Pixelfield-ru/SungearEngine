//
// Created by stuka on 28.12.2024.
//

#pragma once

#include <entt/entity/entity.hpp>
#include <sgcore_export.h>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Serde/Defines.h"

sg_predeclare_serde()

namespace SGCore
{
    struct EntityBaseInfo;
}

namespace SGCore::ECS
{
    // used to workaround linker errors of symbols exporting
    struct SGCORE_EXPORT ComponentBase
    {
        virtual ~ComponentBase() = default;

        ComponentBase() noexcept = default;

        ComponentBase(const ComponentBase& other) noexcept;

        ComponentBase(ComponentBase&& other) noexcept = default;

        virtual entity_t getThisEntity() const noexcept;

        virtual void setActive(bool active) noexcept;

        bool isActive() const noexcept;

        ComponentBase& operator=(const ComponentBase& other);

        // moving entity
        ComponentBase& operator=(ComponentBase&& other) noexcept = default;

    protected:
        bool m_isActive = true;

        entity_t m_thisEntity = entt::null;
    };

    template<typename ComponentRegT, typename ConstComponentRegT, typename ComponentAccessor = decltype([](auto& component) -> decltype(auto) { return *component; })>
    struct Component : ComponentBase
    {
        friend struct SGCore::EntityBaseInfo;

        template<typename>
        friend struct Registry;

        sg_serde_as_friend()

        /**
         * Type of component is registry.
         */
        using reg_t = ComponentRegT;
        /**
         * Cons type of component is registry.
         */
        using const_reg_t = ConstComponentRegT;

        using accessor_t = ComponentAccessor;

        template<typename NestedComponentT = ComponentRegT>
        requires(std::is_base_of_v<Component, NestedComponentT>)
        NestedComponentT& operator*() noexcept
        {
            return static_cast<NestedComponentT&>(*this);
        }

        template<typename NestedComponentT = ConstComponentRegT>
        requires(std::is_base_of_v<Component, NestedComponentT>)
        NestedComponentT& operator*() const noexcept
        {
            return static_cast<const NestedComponentT&>(*this);
        }
    };
}
