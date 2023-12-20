//
// Created by Ilya on 21.12.2023.
//

#ifndef SUNGEARENGINE_UNIQUENAME_H
#define SUNGEARENGINE_UNIQUENAME_H

#include <string>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class UniqueNamesManager;

    struct UniqueName
    {
        friend class UniqueNamesManager;

        Weak<UniqueNamesManager> m_parentUniqueNamesManager;

        [[nodiscard]] std::string getRawName() const noexcept;

        [[nodiscard]] size_t getUniqueID() const noexcept;

        [[nodiscard]] std::string getName() const noexcept;

        template<typename Str = std::string>
        void setRawName(Str&& rawName) noexcept;

    private:
        std::string m_rawName;
        size_t m_uniqueID = 0;
        // m_rawName + m_uniqueID
        std::string m_name;
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

        template<typename Str = std::string>
        void setRawName(Str&& rawName) noexcept
        {
            m_uniqueName.setRawName(std::forward<Str>(rawName));
        }

    private:
        UniqueName m_uniqueName;
    };
}

#endif //SUNGEARENGINE_UNIQUENAME_H
