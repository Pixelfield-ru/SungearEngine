//
// Created by stuka on 25.05.2023.
//

#include "IAsset.h"

long Core::Memory::Assets::IAsset::getLastModified() noexcept
{
    return m_lastModified;
}

void Core::Memory::Assets::IAsset::onModified()
{
    for(const auto& observer : m_observers)
    {
        observer->onAssetModified();
    }
}

void Core::Memory::Assets::IAsset::onPathChanged()
{
    for(const auto& observer : m_observers)
    {
        observer->onAssetPathChanged();
    }
}

void Core::Memory::Assets::IAsset::onDeleted()
{
    for(const auto& observer : m_observers)
    {
        observer->onAssetDeleted();
    }
}

void Core::Memory::Assets::IAsset::onRestored()
{
    for(const auto& observer : m_observers)
    {
        observer->onAssetRestored();
    }
}

void Core::Memory::Assets::IAsset::addObserver(IAssetObserver* observer) noexcept
{
    m_observers.push_back(observer);
}

void Core::Memory::Assets::IAsset::removeObserver(IAssetObserver* observer) noexcept
{
    m_observers.remove(observer);
}

std::string_view Core::Memory::Assets::IAsset::getPath() const noexcept
{
    return m_path;
}
