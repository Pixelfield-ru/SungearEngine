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

    packageTable.set_function("log", [](int logLevel, const std::string& message) {
        switch (static_cast<Logger::Level>(logLevel))
        {
            case Logger::Level::LVL_TRACE:
                SG_LOG_D_UNFORMATTED(message);
                break;

            case Logger::Level::LVL_DEBUG:
                SG_LOG_D_UNFORMATTED(message);
                break;

            case Logger::Level::LVL_INFO:
                SG_LOG_I_UNFORMATTED(message);
                break;

            case Logger::Level::LVL_WARN:
                SG_LOG_W_UNFORMATTED(message);
                break;

            case Logger::Level::LVL_ERROR:
                SG_LOG_E_UNFORMATTED(message);
                break;

            case Logger::Level::LVL_CRITICAL:
                SG_LOG_C_UNFORMATTED(message);
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

