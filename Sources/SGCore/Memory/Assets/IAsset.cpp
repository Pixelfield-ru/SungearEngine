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
    auto it = m_observers.begin();
    while(it != m_observers.end())
    {
        if(auto lockedObserver = it->lock())
        {
            lockedObserver->onAssetModified();
            ++it;
        }
        else
        {
            it = m_observers.erase(it);
        }
    }
}

void SGCore::IAsset::onPathChanged()
{
    auto it = m_observers.begin();
    while(it != m_observers.end())
    {
        if(auto lockedObserver = it->lock())
        {
            lockedObserver->onAssetPathChanged();
            ++it;
        }
        else
        {
            it = m_observers.erase(it);
        }
    }
}

void SGCore::IAsset::onDeleted()
{
    auto it = m_observers.begin();
    while(it != m_observers.end())
    {
        if(auto lockedObserver = it->lock())
        {
            lockedObserver->onAssetDeleted();
            ++it;
        }
        else
        {
            it = m_observers.erase(it);
        }
    }
}

void SGCore::IAsset::onRestored()
{
    auto it = m_observers.begin();
    while(it != m_observers.end())
    {
        if(auto lockedObserver = it->lock())
        {
            lockedObserver->onAssetRestored();
            ++it;
        }
        else
        {
            it = m_observers.erase(it);
        }
    }
}

void SGCore::IAsset::addObserver(const Ref<IAssetObserver>& observer) noexcept
{
    m_observers.push_back(observer);
}

void SGCore::IAsset::removeObserver(const Ref<IAssetObserver>& observer) noexcept
{
    m_observers.remove_if([&observer](auto otherObserver) {
        return !(otherObserver.owner_before(observer) || observer.owner_before(otherObserver));
    });
}

std::filesystem::path SGCore::IAsset::getPath() const noexcept
{
    return m_path;
}
