//
// Created by Ilya on 20.12.2023.
//

#ifndef SUNGEARENGINE_UNIQUENAMESMANAGER_H
#define SUNGEARENGINE_UNIQUENAMESMANAGER_H

#include <string>
#include <unordered_map>
#include <memory>

#include "SGCore/Main/CoreGlobals.h"
#include "UniqueName.h"

namespace SGCore
{
    class UniqueNamesCounter
    {
        friend class UniqueNamesManager;

        std::string m_rawName;
        // count of unique names with this raw name
        size_t m_count = 0;
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

        template<typename Str = std::string>
        void setUniqueNameRawName(UniqueName& uniqueName, Str&& newRawName)
        {
            auto& uniqueNamesCounter = m_uniqueNamesCounters[newRawName];

            uniqueName.m_rawName = uniqueName.m_rawName;
            uniqueName.m_uniqueID = uniqueNamesCounter.m_count;
            uniqueName.m_name = uniqueNamesCounter.m_count == 0 ?
                                uniqueName.m_rawName :
                                uniqueName.m_rawName + " (" + std::to_string(uniqueNamesCounter.m_count) + ")";

            ++uniqueNamesCounter.m_count;
        }

    private:
        std::unordered_map<std::string, UniqueNamesCounter> m_uniqueNamesCounters;
    };
}

#endif //SUNGEARENGINE_UNIQUENAMESMANAGER_H
