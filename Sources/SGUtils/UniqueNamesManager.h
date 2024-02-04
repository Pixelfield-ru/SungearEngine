//
// Created by Ilya on 20.12.2023.
//

#ifndef SUNGEARENGINE_UNIQUENAMESMANAGER_H
#define SUNGEARENGINE_UNIQUENAMESMANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <unordered_set>

#include "UniqueName.h"
#include "Event.h"
#include "EventListener.h"

namespace SGCore
{
    class UniqueNamesCounter
    {
        friend class UniqueNamesManager;

        std::string m_rawName;
        // count of unique names with this raw name
        size_t m_count = 0;
        std::unordered_set<std::string> m_names;
    };

    class UniqueNamesManager : public std::enable_shared_from_this<UniqueNamesManager>
    {
    public:
        UniqueName getUniqueName(const std::string& rawName) noexcept
        {
            UniqueName newUniqueName;

            auto& uniqueNamesCounter = m_uniqueNamesCounters[rawName];

            newUniqueName.m_rawName = rawName;
            newUniqueName.m_uniqueID = uniqueNamesCounter.m_count;
            newUniqueName.m_name = uniqueNamesCounter.m_count == 0 ?
                                   rawName :
                                   rawName + " (" + std::to_string(uniqueNamesCounter.m_count) + ")";

            ++uniqueNamesCounter.m_count;

            return newUniqueName;
        }

        void setUniqueNameRawName(UniqueName& uniqueName, const std::string& newRawName)
        {
            auto& uniqueNamesCounter = m_uniqueNamesCounters[newRawName];

            if(uniqueNamesCounter.m_names.contains(uniqueName.m_rawName + " (" + std::to_string( uniqueName.m_uniqueID) + ")")) return;

            uniqueName.m_rawName = newRawName;
            uniqueName.m_uniqueID = uniqueNamesCounter.m_count;
            uniqueName.m_name = uniqueNamesCounter.m_count == 0 ?
                                uniqueName.m_rawName :
                                uniqueName.m_rawName + " (" + std::to_string(uniqueNamesCounter.m_count) + ")";

            uniqueNamesCounter.m_names.insert(uniqueName.m_rawName + " (" + std::to_string( uniqueName.m_uniqueID) + ")");

            ++uniqueNamesCounter.m_count;

            (*m_someNameChangedEvent)(uniqueName.m_name);
        }

        void subscribeToSomeNameChangedEvent(const EventListener<void(const std::string&)>& eventListener)
        {
            (*m_someNameChangedEvent) += eventListener;
        }

    private:
        Event<void(const std::string& newName)> m_someNameChangedEvent = MakeEvent<void(const std::string& newName)>();

        std::unordered_map<std::string, UniqueNamesCounter> m_uniqueNamesCounters;
    };
}

#endif //SUNGEARENGINE_UNIQUENAMESMANAGER_H
