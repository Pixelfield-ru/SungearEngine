//
// Created by Ilya on 20.12.2023.
//

#pragma once

#include "UniqueName.h"
#include "SGCore/Utils/Signal.h"

namespace SGCore
{
    class SGCORE_EXPORT UniqueNamesManager : public std::enable_shared_from_this<UniqueNamesManager>
    {
    public:
        UniqueName getUniqueName(const std::string& rawName) noexcept;

        void setUniqueNameRawName(UniqueName& uniqueName, const std::string& newRawName);
        void onUniqueNameDestroys(UniqueName& uniqueName);

        void subscribeToSomeNameChangedEvent(Slot<void(const std::string&)>& eventListener);

        void clearCounters() noexcept;

    private:
        class UniqueNamesCounter
        {
            friend class UniqueNamesManager;

            std::string m_rawName;
            std::int64_t m_maxCount = 0;
            std::unordered_set<std::int64_t> m_existingIds;
        };

        Signal<void(const std::string& newName)> onSomeNameChanged;

        std::unordered_map<std::string, UniqueNamesCounter> m_uniqueNamesCounters;
    };
}
