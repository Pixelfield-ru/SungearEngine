//
// Created by stuka on 12.02.2026.
//

#pragma once

#include <sol/state.hpp>

#include "SGCore/Memory/AssetWeakRef.h"
#include "SGCore/Memory/IAssetsRefsResolver.h"
#include "SGCore/Memory/Assets/TextFileAsset.h"

namespace SGCore::Lua
{
    struct Script : IAsset, public IAssetsRefsResolver<Script>
    {
        sg_serde_as_friend()

        sg_implement_type_id(Script, 53)

        sg_assets_refs_resolver_as_friend

        void execute() noexcept;

        AssetRef<TextFileAsset> getFile() noexcept;

    protected:
        void doLoad(const InterpolatedPath& path) override;

        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;

        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept;

    private:
        AssetWeakRef<TextFileAsset> m_scriptFile;

        sol::state m_luaState;
        sol::load_result m_loadResult;

        void initializeLuaState() noexcept;
    };
}
