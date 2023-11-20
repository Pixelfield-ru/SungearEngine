//
// Created by stuka on 25.05.2023.
//

#include "IAsset.h"

long SGCore::IAsset::getLastModified() noexcept
{
    return m_lastModified;
}

void SGCore::IAsset::onModified()
{
    for(const auto& observer : m_observers)
    {
        observer->onAssetModified();
    }
}

void SGCore::IAsset::onPathChanged()
{
    for(const auto& observer : m_observers)
    {
        observer->onAssetPathChanged();
    }
}

void SGCore::IAsset::onDeleted()
{
    for(const auto& observer : m_observers)
    {
        observer->onAssetDeleted();
    }
}

void SGCore::IAsset::onRestored()
{
    for(const auto& observer : m_observers)
    {
        observer->onAssetRestored();
    }
}

void SGCore::IAsset::addObserver(const std::shared_ptr<IAssetObserver>& observer) noexcept
{
    m_observers.push_back(observer);
}

void SGCore::IAsset::removeObserver(const std::shared_ptr<IAssetObserver>& observer) noexcept
{
    m_observers.remove(observer);
}

std::filesystem::path SGCore::IAsset::getPath() const noexcept
{
    return m_path;
}
