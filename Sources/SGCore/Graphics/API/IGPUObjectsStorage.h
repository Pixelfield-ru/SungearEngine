//
// Created by stuka on 07.11.2025.
//

#pragma once

namespace SGCore
{
    struct IGPUObjectsStorage
    {
        virtual ~IGPUObjectsStorage() noexcept;

        /**
         * Recreates all GPU objects
         */
        virtual void recreateAll() noexcept = 0;

        /**
         * Destroys all GPU objects.
         */
        virtual void clear() noexcept = 0;
    };
}