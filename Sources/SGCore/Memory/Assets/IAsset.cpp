//
// Created by stuka on 25.05.2023.
//

#include "IAsset.h"
#include "SGCore/Memory/AssetManager.h"

long SGCore::IAsset::getLastModified() noexcept
{
    return m_lastModified;
}

const SGCore::InterpolatedPath& SGCore::IAsset::getPath() const noexcept
{
    return m_path;
}

const std::string& SGCore::IAsset::getAlias() const noexcept
{
    return m_alias;
}

SGCore::AssetStorageType SGCore::IAsset::storedByWhat() const noexcept
{
    return m_storedBy;
}

SGCore::Ref<SGCore::AssetManager> SGCore::IAsset::getParentAssetManager() const noexcept
{
    return m_parentAssetManager.lock();
}

bool SGCore::IAsset::isLoaded() const noexcept
{
    return m_isLoaded;
}

bool SGCore::IAsset::isSavedBinaryFile() const noexcept
{
    return m_isSavedInBinaryFile;
}

size_t SGCore::IAsset::getHash() const noexcept
{
    if(m_hash != 0) return m_hash;

    switch(m_storedBy)
    {
        case AssetStorageType::BY_PATH:
        {
            m_hash = std::hash<std::filesystem::path>{}(m_path.raw());
            return m_hash;
        };
        case AssetStorageType::BY_ALIAS:
        {
            m_hash = std::hash<std::string>{}(m_alias);
            return m_hash;
        }
    }

    return m_hash;
}

void SGCore::IAsset::reloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept
{
    auto lockedAssetManager = m_parentAssetManager.lock();
    if(!lockedAssetManager) return;

    lockedAssetManager->reloadAssetFromDisk(this, loadPolicy, lazyLoadInThread);
}

void SGCore::IAsset::reloadFromDisk() noexcept
{
    reloadFromDisk(AssetsLoadPolicy::SINGLE_THREADED);
}
