//
// Created by stuka on 19.11.2023.
//

#ifndef SUNGEARENGINE_PATHS_H
#define SUNGEARENGINE_PATHS_H

#include <SGCore/pch.h>
#include <optional>

#include "SGCore/Utils/StringInterpolation/InterpolatedPath.h"
#include "Singleton.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore
{
    struct SGCORE_EXPORT Paths
    {
        static inline const InterpolatedPath undefined_path = "undefined";

        static void init() noexcept;

        std::optional<InterpolatedPath> operator[](const std::string& virtualPath) noexcept;

        InterpolatedPath addPath(const std::string& virtualPath,
                                 const InterpolatedPath& path) noexcept;

        [[nodiscard]] SG_NOINLINE static auto& getDefaultPaths() noexcept
        {
            return *m_defaultPaths;
        }

    private:
        std::unordered_map<std::string, InterpolatedPath> m_paths;

        static inline Ref<Paths> m_defaultPaths { };
    };
}

#endif // SUNGEARENGINE_PATHS_H
