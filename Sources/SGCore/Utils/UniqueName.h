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

        [[nodiscard]] std::string getRawName() const noexcept;

        [[nodiscard]] size_t getUniqueID() const noexcept;

        [[nodiscard]] std::string getName() const noexcept;

        void setRawName(const std::string& rawName) noexcept;

        void attachToManager(const Ref<UniqueNamesManager>& manager) noexcept;

    private:
        Weak<UniqueNamesManager> m_parentUniqueNamesManager;

        std::string m_rawName;
        // -1 IS NOT TYPO
        size_t m_uniqueID = -1;
        // m_rawName + m_uniqueID
        std::string m_name;
    };

    // wrapper class for unique name for inheritance
    class UniqueNameWrapper
    {
    public:
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

        void setRawName(const std::string& rawName) noexcept
        {
            m_uniqueName.setRawName(rawName);
        }

        void attachToUniqueNamesManager(const Ref<UniqueNamesManager>& manager) noexcept
        {
            m_uniqueName.attachToManager(manager);
        }

    private:
        UniqueName m_uniqueName;
    };
}

#endif //SUNGEARENGINE_UNIQUENAME_H
