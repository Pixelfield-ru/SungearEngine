//
// Created by Ilya on 21.12.2023.
//

#ifndef SUNGEARENGINE_UNIQUENAME_H
#define SUNGEARENGINE_UNIQUENAME_H

#include <SGCore/pch.h>

namespace SGCore
{
    class UniqueNamesManager;

    struct UniqueName
    {
        friend class UniqueNamesManager;
        
        UniqueName() = default;
        UniqueName(const UniqueName& other) noexcept;
        UniqueName(const char* rawName) noexcept;
        UniqueName(const std::string& rawName) noexcept;
        UniqueName(UniqueName&&) noexcept = default;

        [[nodiscard]] std::string getRawName() const noexcept;

        [[nodiscard]] size_t getUniqueID() const noexcept;

        [[nodiscard]] std::string getName() const noexcept;

        void setRawName(const std::string& rawName) noexcept;

        void attachToManager(const std::shared_ptr<UniqueNamesManager>& manager) noexcept;
        
        UniqueName& operator=(const UniqueName& other) noexcept;
        UniqueName& operator=(UniqueName&&) noexcept = default;
        
        UniqueName& operator=(const char* rawName) noexcept;
        UniqueName& operator=(const std::string& rawName) noexcept;
        
        bool operator==(const UniqueName& other) const noexcept;
        bool operator!=(const UniqueName& other) const noexcept;
        bool operator==(const char* name) const noexcept;
        bool operator!=(const char* name) const noexcept;
        bool operator==(const std::string& name) const noexcept;
        bool operator!=(const std::string& name) const noexcept;

    private:
        std::weak_ptr<UniqueNamesManager> m_parentUniqueNamesManager;

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

        void attachToUniqueNamesManager(const std::shared_ptr<UniqueNamesManager>& manager) noexcept
        {
            m_uniqueName.attachToManager(manager);
        }

    private:
        UniqueName m_uniqueName;
    };
}

#endif //SUNGEARENGINE_UNIQUENAME_H
