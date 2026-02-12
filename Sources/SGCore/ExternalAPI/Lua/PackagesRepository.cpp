//
// Created by stuka on 12.02.2026.
//

#include "PackagesRepository.h"

#include <ranges>

void SGCore::Lua::PackagesRepository::addPackage(const AssetRef<Package>& package) noexcept
{
    storage()[package->getPackageName()] = package;
}

void SGCore::Lua::PackagesRepository::removePackage(const std::string& packageName) noexcept
{
    storage().erase(packageName);
}

bool SGCore::Lua::PackagesRepository::hasPackage(const std::string& packageName) noexcept
{
    return storage().contains(packageName);
}

void SGCore::Lua::PackagesRepository::loadAllPackagesInLua(sol::state& luaState) noexcept
{
    for(const auto& package : storage() | std::views::values)
    {
        package->loadInState(luaState);
    }
}

std::unordered_map<std::string, SGCore::AssetRef<SGCore::Lua::Package>>& SGCore::Lua::PackagesRepository::storage() noexcept
{
    static std::unordered_map<std::string, AssetRef<Package>> s_storage;
    return s_storage;
}
