//
// Created by Ilya on 21.12.2023.
//

#include "UniqueName.h"
#include "UniqueNamesManager.h"

SGCore::UniqueName::UniqueName(const UniqueName& other) noexcept
{
    operator=(other);
}

SGCore::UniqueName::UniqueName(const char* rawName) noexcept
{
    setRawName(rawName);
}

SGCore::UniqueName::UniqueName(const std::string& rawName) noexcept
{
    setRawName(rawName);
}

std::string SGCore::UniqueName::getRawName() const noexcept
{
    return m_rawName;
}

size_t SGCore::UniqueName::getUniqueID() const noexcept
{
    return m_uniqueID;
}

std::string SGCore::UniqueName::getName() const noexcept
{
    return m_name;
}

void SGCore::UniqueName::setRawName(const std::string& rawName) noexcept
{
    if(auto lockedParent = m_parentUniqueNamesManager.lock())
    {
        lockedParent->setUniqueNameRawName(*this, rawName);
    }
    else
    {
        m_rawName = rawName;
        m_name = rawName;;
    }
}

void SGCore::UniqueName::attachToManager(const std::shared_ptr<UniqueNamesManager>& manager) noexcept
{
    m_parentUniqueNamesManager = manager;

    // update the name
    setRawName(m_rawName);
}

std::shared_ptr<SGCore::UniqueNamesManager> SGCore::UniqueName::getNamesManager() const noexcept
{
    return m_parentUniqueNamesManager.lock();
}

SGCore::UniqueName& SGCore::UniqueName::operator=(const SGCore::UniqueName& other) noexcept
{
    if(this == std::addressof(other)) return *this;
    
    m_parentUniqueNamesManager = other.m_parentUniqueNamesManager;
    
    setRawName(other.m_rawName);
    
    return *this;
}

SGCore::UniqueName& SGCore::UniqueName::operator=(const char* rawName) noexcept
{
    setRawName(rawName);
    
    return *this;
}

SGCore::UniqueName& SGCore::UniqueName::operator=(const std::string& rawName) noexcept
{
    setRawName(rawName);
    
    return *this;
}

bool SGCore::UniqueName::operator==(const SGCore::UniqueName& other) const noexcept
{
    return m_name == other.m_name;
}

bool SGCore::UniqueName::operator!=(const SGCore::UniqueName& other) const noexcept
{
    return !(*this == other);
}

bool SGCore::UniqueName::operator==(const char* name) const noexcept
{
    return m_name == name;
}

bool SGCore::UniqueName::operator!=(const char* name) const noexcept
{
    return m_name != name;
}

bool SGCore::UniqueName::operator==(const std::string& name) const noexcept
{
    return m_name == name;
}

bool SGCore::UniqueName::operator!=(const std::string& name) const noexcept
{
    return m_name != name;
}
