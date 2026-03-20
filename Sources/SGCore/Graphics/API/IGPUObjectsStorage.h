//
// Created by stuka on 07.11.2025.
//

#pragma once

#include <sgcore_export.h>

namespace SGCore
{
    struct SGCORE_EXPORT IGPUObjectsStorage
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