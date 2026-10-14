//
// Created by stuka on 14.10.2026.
//

#pragma once

#include <memory>
#include <vector>

#include "pixelfield/core/utility/type_traits.h"
#include "pixelfield/core/utility/assert.h"

namespace pf::core
{
    // todo: use pmr
    struct TypeMap final
    {
        template<typename T, typename... Args>
        T& emplace(Args&&... args) noexcept
        {
            const auto index = typeIndex<T>();
            if(index >= storage_.size())
            {
                storage_.resize(index + 1);
            }

            auto storage = std::make_unique<Storage<T>>(std::forward<Args>(args)...);
            auto& value = storage->value;
            storage_[index] = std::move(storage);

            return value;
        }

        template<typename T>
        const T& read() const noexcept
        {
            const auto index = typeIndex<T>();
            pf_assert(index < storage_.size(), "Index out of bounds");

            return static_cast<Storage<T>*>(storage_[index].get())->value;
        }

        template<typename T>
        T& write() noexcept
        {
            const auto index = typeIndex<T>();
            pf_assert(index < storage_.size(), "Index out of bounds");

            return static_cast<Storage<T>*>(storage_[index].get())->value;
        }

        template<typename T>
        [[nodiscard]] bool hasType() const noexcept
        {
            const auto index = typeIndex<T>();

            return index < storage_.size();
        }

    private:
        struct Erased
        {
            virtual ~Erased() noexcept = default;
        };

        template<typename T>
        struct Storage final : Erased
        {
            template<typename... Args>
            explicit Storage(Args&&... args) : value(std::forward<Args>(args)...) { }

            T value;
        };

        std::vector<std::unique_ptr<Erased>> storage_;
    };
}
