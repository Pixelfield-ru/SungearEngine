//
// Created by stuka on 02.07.2023.
//

#include "IAssetObserver.h"

bool Core::Memory::Assets::IAssetObserver::isAssetModifiedChecking() const noexcept
{
    return m_assetModifiedChecking;
}

void Core::Memory::Assets::IAssetObserver::setAssetModifiedChecking(const bool& checking) noexcept
{
    m_assetModifiedChecking = checking;

    if(m_assetModifiedChecking)
    {
        onAssetModified();
    }
}

// ----------

bool Core::Memory::Assets::IAssetObserver::isAssetPathChangedChecking() const noexcept
{
    return m_assetPathChangedChecking;
}

void Core::Memory::Assets::IAssetObserver::setAssetPathChangedChecking(const bool& checking) noexcept
{
    m_assetPathChangedChecking = checking;

    if(m_assetPathChangedChecking)
    {
        onAssetPathChanged();
    }
}

// ---------

bool Core::Memory::Assets::IAssetObserver::isAssetDeletedChecking() const noexcept
{
    return m_assetDeletedChecking;
}

void Core::Memory::Assets::IAssetObserver::setAssetDeletedChecking(const bool& checking) noexcept
{
    m_assetDeletedChecking = checking;

    if(m_assetDeletedChecking)
    {
        onAssetDeleted();
    }
}

// ---------

bool Core::Memory::Assets::IAssetObserver::isAssetRestoredChecking() const noexcept
{
    return m_assetRestoredChecking;
}

void Core::Memory::Assets::IAssetObserver::setAssetRestoredChecking(const bool& checking) noexcept
{
    m_assetRestoredChecking = checking;

    if(m_assetRestoredChecking)
    {
        onAssetRestored();
    }
}
