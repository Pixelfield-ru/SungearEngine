//
// Created by stuka on 12.02.2026.
//

#pragma once

#include "Package.h"

namespace SGCore::Lua
{
    struct PackagesRepository
    {
        static void addPackage(const AssetRef<Package>& package) noexcept;
        static void removePackage(const std::string& packageName) noexcept;
        static bool hasPackage(const std::string& packageName) noexcept;

        static void loadAllPackagesInLua(sol::state& luaState) noexcept;

    private:
        static std::unordered_map<std::string, AssetRef<Package>>& storage() noexcept;
    };
}
