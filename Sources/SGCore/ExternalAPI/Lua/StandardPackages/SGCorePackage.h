//
// Created by stuka on 12.02.2026.
//

#pragma once

#include "SGCore/ExternalAPI/Lua/Package.h"

namespace SGCore::Lua
{
    struct SGCorePackage final : Package
    {
        sg_declare_lua_package(SGCore)

        sg_serde_as_friend()

        sg_implement_type_id(SGCorePackage, 54)

    private:
        void doLoadInState(sol::state& luaState, sol::unsafe_function_result& packageResult) noexcept override;
    };
}
