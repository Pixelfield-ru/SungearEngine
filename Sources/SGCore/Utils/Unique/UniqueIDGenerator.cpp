//
// Created by stuka on 25.09.2025.
//

#include "UniqueIDGenerator.h"

#include "UniqueID.h"

void SGCore::UniqueIDGenerator::generate(UniqueID& id) noexcept
{
    if(id.m_valid) return;

    id.m_valid = true;
    id.m_id = getFreeID();
}

void SGCore::UniqueIDGenerator::pushFreeID(size_t id) noexcept
{
    std::lock_guard guard(m_freeIDsMutex);
    m_freeIDs.push_back(id);
}

size_t SGCore::UniqueIDGenerator::getFreeID() noexcept
{
    {
        std::lock_guard guard(m_freeIDsMutex);
        if(!m_freeIDs.empty())
        {
            const auto freeID = m_freeIDs.back();
            m_freeIDs.pop_back();
            std::cout << "got free id" << std::endl;
            return freeID;
        }
    }

    const size_t id = m_maxID++;

    return id;
}
