//
// Created by Ilya on 18.07.2024.
//

#include "UniqueNamesManager.h"

SGCore::UniqueName SGCore::UniqueNamesManager::getUniqueName(const std::string& rawName) noexcept
{
    UniqueName newUniqueName;
    newUniqueName.setRawName(rawName);

    newUniqueName.attachToManager(shared_from_this());

    return newUniqueName;
}

void SGCore::UniqueNamesManager::setUniqueNameRawName(UniqueName& uniqueName, const std::string& newRawName)
{
    auto& uniqueNamesCounter = m_uniqueNamesCounters[newRawName];

    {
        auto& lastUniqueNamesCounter = m_uniqueNamesCounters[uniqueName.m_rawName];

        if (newRawName != uniqueName.m_rawName)
        {
            lastUniqueNamesCounter.m_existingIds.erase(uniqueName.m_uniqueID);
        }
    }

    const auto lastFullName = uniqueName.m_name;

    if(uniqueNamesCounter.m_existingIds.contains(uniqueName.m_uniqueID) && uniqueName.m_rawName == newRawName) return;

    uniqueName.m_rawName = newRawName;

    if(uniqueNamesCounter.m_existingIds.empty())
    {
        uniqueName.m_name = uniqueName.m_rawName;
        uniqueName.m_uniqueID = 0;
    }
    else
    {
        for (std::int64_t i = 0; i <= uniqueNamesCounter.m_maxCount; ++i)
        {
            if (!uniqueNamesCounter.m_existingIds.contains(i))
            {
                uniqueName.m_uniqueID = i;

                uniqueName.m_name = i == 0 ?
                                    uniqueName.m_rawName :
                                    uniqueName.m_rawName + " (" + std::to_string(i) + ")";

                break;
            }
        }
    }

    uniqueNamesCounter.m_existingIds.insert(uniqueName.m_uniqueID);

    if(uniqueName.m_name != uniqueName.m_rawName)
    {
        auto& newNamesCounter = m_uniqueNamesCounters[uniqueName.m_name];
        ++newNamesCounter.m_maxCount;
        newNamesCounter.m_rawName = uniqueName.m_name;
        newNamesCounter.m_existingIds.insert(0);
    }

    ++uniqueNamesCounter.m_maxCount;

    onSomeNameChanged(uniqueName.m_name);
}

void SGCore::UniqueNamesManager::onUniqueNameDestroys(SGCore::UniqueName& uniqueName)
{
    /*std::printf("unique name with name '%s' (raw name: '%s', idx: '%lli') destroys...\n", uniqueName.m_name.c_str(),
                uniqueName.m_rawName.c_str(), uniqueName.m_uniqueID);*/

    {
        auto& counter = m_uniqueNamesCounters[uniqueName.m_rawName];
        counter.m_existingIds.erase(uniqueName.m_uniqueID);
    }

    {
        auto& counter = m_uniqueNamesCounters[uniqueName.m_name];
        counter.m_existingIds.erase(0);
    }
}

void SGCore::UniqueNamesManager::subscribeToSomeNameChangedEvent(SGCore::Slot<void(const std::string&)>& eventListener)
{
    onSomeNameChanged += eventListener;
}

void SGCore::UniqueNamesManager::clearCounters() noexcept
{
    m_uniqueNamesCounters.clear();
}
