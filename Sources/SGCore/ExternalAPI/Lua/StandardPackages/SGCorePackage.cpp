//
// Created by stuka on 12.02.2026.
//

#include "SGCorePackage.h"

#include "SGCore/Main/CoreMain.h"

/*struct Player
{
    std::string m_name = "alesha shveev";
};*/

void SGCore::Lua::SGCorePackage::doLoadInState(sol::state& luaState, function_result& packageResult) noexcept
{
    sol::table packageTable = packageResult;

    packageTable.set_function("log", [](const std::string& tag, int logLevel, const std::string& message) {
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

    /*static auto makePlayer = [](const std::string& name) {
        return Player { name };
    };

    auto playerType = packageTable.new_usertype<Player>("Player", sol::no_constructor);
    playerType["name"] = &Player::m_name;
    playerType["new"] = makePlayer;*/
}

