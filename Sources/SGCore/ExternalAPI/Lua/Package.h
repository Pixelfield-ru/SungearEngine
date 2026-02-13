//
// Created by stuka on 12.02.2026.
//

#pragma once

#include <sol/state.hpp>

#include "SGCore/Memory/AssetWeakRef.h"
#include "SGCore/Memory/IAssetsRefsResolver.h"
#include "SGCore/Memory/Assets/TextFileAsset.h"

#define sg_declare_lua_package(PackageName) \
    static const inline std::string s_packageName = #PackageName; \
    const std::string& getPackageName() const noexcept override { return s_packageName; }

namespace SGCore::Lua
{
    struct Package : IAsset, public IAssetsRefsResolver<Package>
    {
        sg_assets_refs_resolver_as_friend

        void loadInState(sol::state& luaState, sol::unsafe_function_result& packageResult) noexcept;

        AssetRef<TextFileAsset> getFile() noexcept;

        virtual const std::string& getPackageName() const noexcept = 0;

    protected:
        AssetWeakRef<TextFileAsset> m_packageFile;

        void doLoad(const InterpolatedPath& path) override;

        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;

        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept;

        virtual void doLoadInState(sol::state& luaState, sol::unsafe_function_result& packageResult) noexcept = 0;
    };
}
