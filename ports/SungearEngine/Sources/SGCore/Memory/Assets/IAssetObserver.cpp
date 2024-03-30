//
// Created by stuka on 02.07.2023.
//

#include "IAssetObserver.h"

bool SGCore::IAssetObserver::isAssetModifiedChecking() const noexcept
{
    return m_assetModifiedChecking;
}

void SGCore::IAssetObserver::setAssetModifiedChecking(const bool& checking) noexcept
{
    m_assetModifiedChecking = checking;

    if(m_assetModifiedChecking)
    {
        onAssetModified();
    }
}

// ----------

bool SGCore::IAssetObserver::isAssetPathChangedChecking() const noexcept
{
    return m_assetPathChangedChecking;
}

void SGCore::IAssetObserver::setAssetPathChangedChecking(const bool& checking) noexcept
{
    m_assetPathChangedChecking = checking;

    if(m_assetPathChangedChecking)
    {
        onAssetPathChanged();
    }
}

// ---------

bool SGCore::IAssetObserver::isAssetDeletedChecking() const noexcept
{
    return m_assetDeletedChecking;
}

void SGCore::IAssetObserver::setAssetDeletedChecking(const bool& checking) noexcept
{
    m_assetDeletedChecking = checking;

    if(m_assetDeletedChecking)
    {
        onAssetDeleted();
    }
}

// ---------

bool SGCore::IAssetObserver::isAssetRestoredChecking() const noexcept
{
    return m_assetRestoredChecking;
}

void SGCore::IAssetObserver::setAssetRestoredChecking(const bool& checking) noexcept
{
    m_assetRestoredChecking = checking;

    if(m_assetRestoredChecking)
    {
        onAssetRestored();
    }
}
