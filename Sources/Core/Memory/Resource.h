//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_RESOURCE_H
#define NATIVECORE_RESOURCE_H

#include <cassert>
#include <memory>

namespace Core::Memory
{
    template<typename ResourceTag, typename ResourceT>
    class Resource
    {
    private:
        ResourceT resource{};

        void free_resource() noexcept;

    public:
        Resource() noexcept = default;

        // copy
        explicit Resource(ResourceT res) noexcept : resource { res } { }

        Resource(const Resource&) = delete;
        Resource& operator=(const Resource&) = delete;

        // move
        inline Resource(Resource&& other) noexcept: resource { other.resource }
        {
            other.resource = {};
        }

        ~Resource()
        {
            free_resource();
        }

        Resource& operator=(Resource&& other) noexcept
        {
            assert(this != std::addressof(other));

            free_resource();
            resource = other.resource;
            other.resource = {};
            return *this;
        }
    };
}

#endif //NATIVECORE_RESOURCE_H
