//
// Created by Ilya on 21.12.2023.
//

#ifndef SUNGEARENGINE_UNIQUENAME_H
#define SUNGEARENGINE_UNIQUENAME_H

#include "SGCore/Serde/Defines.h"

#include <string>
#include <memory>
#include <cstdint>

sg_predeclare_serde()

namespace SGCore
{
    class UniqueNamesManager;

    struct UniqueName
    {
        sg_serde_as_friend()

        friend class UniqueNamesManager;

        UniqueName() = default;
        UniqueName(const UniqueName& other) noexcept;
        UniqueName(const char* rawName) noexcept;
        UniqueName(const std::string& rawName) noexcept;
        UniqueName(UniqueName&&) noexcept = default;
        ~UniqueName() noexcept;

        [[nodiscard]] std::string getRawName() const noexcept;

        [[nodiscard]] size_t getUniqueID() const noexcept;

        [[nodiscard]] std::string getName() const noexcept;

        void setRawName(const std::string& rawName) noexcept;

        void attachToManager(const std::shared_ptr<UniqueNamesManager>& manager) noexcept;
        std::shared_ptr<UniqueNamesManager> getNamesManager() const noexcept;

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
        bool operator==(std::string_view name) const noexcept;
        bool operator!=(std::string_view name) const noexcept;

        operator std::string() const noexcept;

    private:
        std::weak_ptr<UniqueNamesManager> m_parentUniqueNamesManager;

        std::string m_rawName;
        // -1 IS NOT TYPO
        std::int64_t m_uniqueID = -1;
        // m_rawName + m_uniqueID
        std::string m_name;
    };

    // wrapper class for unique name for inheritance
    class UniqueNameWrapper
    {
        sg_serde_as_friend()
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

    protected:
        UniqueName m_uniqueName;

    private:
        virtual void dummy() const noexcept { }
    };
}

#endif //SUNGEARENGINE_UNIQUENAME_H
