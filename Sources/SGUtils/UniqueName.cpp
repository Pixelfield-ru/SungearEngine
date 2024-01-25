//
// Created by Ilya on 21.12.2023.
//

#include "UniqueName.h"
#include "UniqueNamesManager.h"

std::string SGUtils::UniqueName::getRawName() const noexcept
{
    return m_rawName;
}

size_t SGUtils::UniqueName::getUniqueID() const noexcept
{
    return m_uniqueID;
}

std::string SGUtils::UniqueName::getName() const noexcept
{
    return m_name;
}

void SGUtils::UniqueName::setRawName(const std::string& rawName) noexcept
{
    if(auto lockedParent = m_parentUniqueNamesManager.lock())
    {
        lockedParent->setUniqueNameRawName(*this, rawName);
    }
    else
    {
        m_rawName = rawName;
    }
}

void SGUtils::UniqueName::attachToManager(const std::shared_ptr<UniqueNamesManager>& manager) noexcept
{
    m_parentUniqueNamesManager = manager;

    // update the name
    setRawName(m_rawName);
}