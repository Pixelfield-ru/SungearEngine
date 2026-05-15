//
// Created by stuka on 16.05.2026.
//

#pragma once

#include "SGCore/Utils/StaticTypeID.h"

#define sg_implement_singleton_component(ComponentT) \
    sg_implement_nonvirtual_type_id(ComponentT);

namespace SGCore::ECS
{
    template<typename DerivedT>
    struct SingletonComponent
    {
    private:
        static inline bool staticInit = []() {
            return true;
        }();

        void assertDerived() const noexcept
        {
            static_assert(
                requires { DerivedT::getTypeIDStatic(); },
                "Please, implement singleton component sg_implement_singleton_component() macro."
            );
        }
    };
}