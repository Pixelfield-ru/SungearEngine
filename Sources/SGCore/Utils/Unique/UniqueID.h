//
// Created by stuka on 25.09.2025.
//

#pragma once

#include <atomic>

namespace SGCore
{
    struct UniqueIDGenerator;

    struct UniqueID
    {
        friend struct UniqueIDGenerator;

        ~UniqueID() noexcept;
        UniqueID() = default;
        UniqueID(const UniqueID& other) noexcept;
        UniqueID(UniqueID&& other) noexcept;

        size_t id() const noexcept;
        [[nodiscard]] bool valid() const noexcept;

        UniqueID& operator=(const UniqueID& other) noexcept;
        UniqueID& operator=(UniqueID&& other) noexcept;

    private:
        std::atomic<size_t> m_id = 0;
        std::atomic<bool> m_valid = false;
    };
}
