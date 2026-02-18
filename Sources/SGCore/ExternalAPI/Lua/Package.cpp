//
// Created by stuka on 12.02.2026.
//

#include "Package.h"

#include "SGCore/Memory/AssetManager.h"

void SGCore::Lua::Package::loadInState(sol::state& luaState, function_result& packageResult) noexcept
{
    auto lockedFile = m_packageFile.lock();
    if(!lockedFile)
    {
        LOG_E(SGCORE_TAG,
              "Cannot load Lua package '{}' from file '{}': this file does not exist.",
              getPackageName(),
              Utils::toUTF8(getPath().resolved().u16string()))
        return;
    }

    // auto result = luaState.script(lockedFile->getData());

    doLoadInState(luaState, packageResult);

    /*if(!result.valid())
    {
        const sol::error err = result;

        LOG_E(SGCORE_TAG,
              "Lua package script compilation error:\n{}\nIn file: '{}'",
              err.what(),
              Utils::toUTF8(getPath().resolved().u16string()));
    }*/
}

SGCore::AssetRef<SGCore::TextFileAsset> SGCore::Lua::Package::getFile() noexcept
{
    return m_packageFile.lock();
}

void SGCore::Lua::Package::doLoad(const InterpolatedPath& path)
{
    m_packageFile = getParentAssetManager()->loadAsset<TextFileAsset>(path);
}

void SGCore::Lua::Package::doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept
{
    // nothing to do...
}

void SGCore::Lua::Package::doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept
{
    auto lockedFile = m_packageFile.lock();
    if(!lockedFile)
    {
        doLoad(getPath());
        return;
    }

    lockedFile->reloadFromDisk(loadPolicy, lazyLoadInThread);
}

void SGCore::Lua::Package::onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept
{
    AssetManager::resolveWeakAssetReference(updatedAssetManager, m_packageFile);
}
