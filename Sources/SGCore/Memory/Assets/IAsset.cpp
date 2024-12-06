//
// Created by stuka on 25.05.2023.
//

#include "IAsset.h"

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
