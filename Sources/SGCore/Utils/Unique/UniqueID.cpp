//
// Created by stuka on 25.09.2025.
//

#include "UniqueID.h"

#include "UniqueIDGenerator.h"

SGCore::UniqueID::~UniqueID() noexcept
{
    if(m_valid)
    {
        UniqueIDGenerator::pushFreeID(m_id);
    }
}

SGCore::UniqueID::UniqueID(const UniqueID& other) noexcept
{
    if(other.m_valid)
    {
        UniqueIDGenerator::generate(*this);
    }
}

SGCore::UniqueID::UniqueID(UniqueID&& other) noexcept
{
    const size_t otherID = other.m_id;

    m_id = otherID;
    m_valid = other.m_valid.load();

    if(other.m_valid)
    {
        UniqueIDGenerator::pushFreeID(otherID);
        other.m_valid = false;
    }
}

size_t SGCore::UniqueID::id() const noexcept
{
    return m_id;
}

bool SGCore::UniqueID::valid() const noexcept
{
    return m_valid;
}

SGCore::UniqueID& SGCore::UniqueID::operator=(const UniqueID& other) noexcept
{
    if(this == &other) return *this;

    if(m_valid) return *this;

    if(other.m_valid)
    {
        UniqueIDGenerator::generate(*this);
    }

    return *this;
}

SGCore::UniqueID& SGCore::UniqueID::operator=(UniqueID&& other) noexcept
{
    if(this == &other) return *this;

    if(m_valid)
    {
        UniqueIDGenerator::pushFreeID(m_id);
    }

    const size_t otherID = other.m_id;

    m_id = otherID;
    m_valid = other.m_valid.load();
    other.m_valid = false;

    return *this;
}
