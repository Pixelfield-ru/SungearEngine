//
// Created by Ilya on 21.12.2023.
//

#include "UniqueName.h"
#include "UniqueNamesManager.h"

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

template<typename Str>
void SGCore::UniqueName::setRawName(Str&& rawName) noexcept
{
    if(auto lockedParent = m_parentUniqueNamesManager.lock())
    {
        lockedParent->setUniqueNameRawName(*this, std::forward<Str>(rawName));
    }
}
