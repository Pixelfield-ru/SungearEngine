//
// Created by Ilya on 20.12.2023.
//

#ifndef SUNGEARENGINE_UNQIUENAMESMANAGER_H
#define SUNGEARENGINE_UNQIUENAMESMANAGER_H

#include <string>
#include <unordered_map>

namespace SGCore
{
    struct UniqueName
    {
        friend class UniqueNamesManager;

        [[nodiscard]] auto getRawName() const noexcept
        {
            return m_rawName;
        }

        [[nodiscard]] auto getUniqueID() const noexcept
        {
            return m_uniqueID;
        }

        [[nodiscard]] auto getName() const noexcept
        {
            return m_name;
        }

    private:
        std::string m_rawName;
        size_t m_uniqueID = 0;
        // m_rawName + m_uniqueID
        std::string m_name;
    };

    class UniqueNamesCounter
    {
        friend class UniqueNamesManager;

        std::string m_rawName;
        // count of unique names with this raw name
        size_t m_count = 0;
    };

    class UniqueNamesManager
    {
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

    private:
        std::unordered_map<std::string, UniqueNamesCounter> m_uniqueNamesCounters;
    };

    // wrapper class for unique name for inheritance
    class UniqueNameWrapper
    {
        [[nodiscard]] auto getRawName() const noexcept
        {
            return m_uniqueName.getRawName();
        }

        [[nodiscard]] auto getUniqueID() const noexcept
        {
            return m_uniqueName.getUniqueID();
        }

        [[nodiscard]] auto getName() const noexcept
        {
            return m_uniqueName.getName();
        }

    private:
        UniqueName m_uniqueName;
    };
}

#endif //SUNGEARENGINE_UNQIUENAMESMANAGER_H
