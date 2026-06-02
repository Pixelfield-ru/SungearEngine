//
// Created by stuka on 16.05.2026.
//

#pragma once

#include "SGCore/Utils/StaticTypeID.h"
#include "Registry.h"

#define sg_implement_singleton_component(ComponentT) \
    sg_implement_nonvirtual_type_id(ComponentT);

namespace SGCore::ECS
{
    template<typename DerivedT>
    struct SingletonComponent
    {
        SingletonComponent()
        {
            static bool staticInit = []() {
                registry_t::registerSingleton<DerivedT>();

                return true;
            }();
        }

        ~SingletonComponent() noexcept
        {
            static_assert(
                requires { DerivedT::getTypeIDStatic(); },
                "Please, implement singleton component sg_implement_singleton_component() macro."
            );
        }
    };
}