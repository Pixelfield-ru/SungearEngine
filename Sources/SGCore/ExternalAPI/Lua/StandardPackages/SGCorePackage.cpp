//
// Created by stuka on 12.02.2026.
//

#include "SGCorePackage.h"

#include "SGCore/Main/CoreMain.h"

void SGCore::Lua::SGCorePackage::doLoadInState(sol::state& luaState) noexcept
{
    luaState.script("package.path = '" + Utils::toUTF8(CoreMain::getSungearEngineRootPath().u16string()) + "/Resources/lua/?.lua;' .. package.path");

    luaState.set_function("SGCore_log_impl", [](const std::string& tag, int logLevel, const std::string& message) {
        switch (static_cast<Logger::Level>(logLevel))
        {
            case Logger::Level::LVL_INFO:
                LOG_I_UNFORMATTED(tag, message)
                break;

            case Logger::Level::LVL_DEBUG:
                LOG_D_UNFORMATTED(tag, message)
                break;

            case Logger::Level::LVL_WARN:
                LOG_W_UNFORMATTED(tag, message)
                break;

            case Logger::Level::LVL_ERROR:
                LOG_E_UNFORMATTED(tag, message)
                break;

            case Logger::Level::LVL_CRITICAL:
                LOG_C_UNFORMATTED(tag, message)
                break;
        }
    });
}
