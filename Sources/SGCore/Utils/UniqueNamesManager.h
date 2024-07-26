//
// Created by Ilya on 20.12.2023.
//

#ifndef SUNGEARENGINE_UNIQUENAMESMANAGER_H
#define SUNGEARENGINE_UNIQUENAMESMANAGER_H

#include <SGCore/pch.h>

#include "UniqueName.h"
#include "Event.h"
#include "EventListener.h"

namespace SGCore
{
    class UniqueNamesCounter
    {
        friend class UniqueNamesManager;

        std::string m_rawName;
        std::int64_t m_maxCount = 0;
        std::unordered_set<std::int64_t> m_existingIds;
    };

    class UniqueNamesManager : public std::enable_shared_from_this<UniqueNamesManager>
    {
    public:
        UniqueName getUniqueName(const std::string& rawName) noexcept;

        void setUniqueNameRawName(UniqueName& uniqueName, const std::string& newRawName);
        void onUniqueNameDestroys(UniqueName& uniqueName);

        void subscribeToSomeNameChangedEvent(const EventListener<void(const std::string&)>& eventListener);

        void clearCounters() noexcept;

    private:
        Event<void(const std::string& newName)> onSomeNameChanged;

        std::unordered_map<std::string, UniqueNamesCounter> m_uniqueNamesCounters;
    };
}

#endif //SUNGEARENGINE_UNIQUENAMESMANAGER_H
