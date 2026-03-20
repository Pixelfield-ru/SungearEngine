//
// Created by stuka on 20.03.2026.
//

#include "StaticTypeID.h"

bool SGCore::StaticTypeIDsContainer::isTypeIDExists(const size_t& typeID) noexcept
{
    return getExistingTypeIDs().contains(typeID);
}

void SGCore::StaticTypeIDsContainer::addTypeID(size_t typeID) noexcept
{
    getExistingTypeIDs().insert(typeID);
}

std::set<size_t>& SGCore::StaticTypeIDsContainer::getExistingTypeIDs() noexcept
{
    static std::set<size_t> m_existingTypeIDs {};

    return m_existingTypeIDs;
}
