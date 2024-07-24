//
// Created by Ilya on 18.07.2024.
//

#include "UniqueNamesManager.h"

SGCore::UniqueName SGCore::UniqueNamesManager::getUniqueName(const std::string& rawName) noexcept
{
    UniqueName newUniqueName;

    auto& uniqueNamesCounter = m_uniqueNamesCounters[rawName];

    newUniqueName.m_rawName = rawName;
    newUniqueName.m_uniqueID = uniqueNamesCounter.m_count;
    newUniqueName.m_name = uniqueNamesCounter.m_count == 0 ?
                           rawName :
                           rawName + " (" + std::to_string(uniqueNamesCounter.m_count) + ")";

po    ++uniqueNamesCounter.m_count;

    // TODO: MAYBE INCORRECT BEHAVIOUR THERE BECAUSE OF COPY OPERATOR
    return newUniqueName;
}

void SGCore::UniqueNamesManager::setUniqueNameRawName(UniqueName& uniqueName, const std::string& newRawName)
{
    auto& uniqueNamesCounter = m_uniqueNamesCounters[newRawName];
    auto& lastUniqueNamesCounter = m_uniqueNamesCounters[uniqueName.m_rawName];

    if(newRawName != uniqueName.m_rawName)
    {
        --lastUniqueNamesCounter.m_count;
        lastUniqueNamesCounter.m_count = std::max(0ll, lastUniqueNamesCounter.m_count);
    }

    if(uniqueNamesCounter.m_names.contains(uniqueName.m_rawName + " (" + std::to_string( uniqueName.m_uniqueID) + ")")) return;

    uniqueName.m_rawName = newRawName;
    uniqueName.m_uniqueID = uniqueNamesCounter.m_count;
    uniqueName.m_name = uniqueNamesCounter.m_count == 0 ?
                        uniqueName.m_rawName :
                        uniqueName.m_rawName + " (" + std::to_string(uniqueNamesCounter.m_maxCount) + ")";

    uniqueNamesCounter.m_names.insert(uniqueName.m_rawName + " (" + std::to_string( uniqueName.m_uniqueID) + ")");

    ++uniqueNamesCounter.m_count;
    ++uniqueNamesCounter.m_maxCount;

    onSomeNameChanged(uniqueName.m_name);
}

void SGCore::UniqueNamesManager::subscribeToSomeNameChangedEvent(
        const SGCore::EventListener<void(const std::string&)>& eventListener)
{
    onSomeNameChanged += eventListener;
}
