//
// Created by stuka on 20.03.2026.
//

#include "StaticTypeID.h"

bool SGCore::StaticTypeIDsContainer::isTypeIDExists(std::uint64_t typeID) noexcept
{
    return getExistingTypeIDs().contains(typeID);
}

void SGCore::StaticTypeIDsContainer::addTypeID(std::uint64_t typeID) noexcept
{
    getExistingTypeIDs().insert(typeID);
}

std::set<std::uint64_t>& SGCore::StaticTypeIDsContainer::getExistingTypeIDs() noexcept
{
    static std::set<std::uint64_t> m_existingTypeIDs {};

    return m_existingTypeIDs;
}
