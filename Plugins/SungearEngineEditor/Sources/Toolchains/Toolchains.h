//
// Created by Ilya on 17.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_TOOLCHAINS_H
#define SUNGEARENGINEEDITOR_TOOLCHAINS_H

#include <vector>
#include <SGCore/Utils/Utils.h>
#include <SGCore/Main/CoreGlobals.h>
#include "Toolchain.h"

namespace SGE
{
    struct Toolchains
    {
        SG_NOINLINE static std::vector<SGCore::Ref<Toolchain>> getToolchains() noexcept;

    private:
        static inline std::vector<SGCore::Ref<Toolchain>> m_toolchains { };
    };
}

#endif //SUNGEARENGINEEDITOR_TOOLCHAINS_H
