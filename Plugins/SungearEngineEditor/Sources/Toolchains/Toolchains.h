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
        std::vector<SGCore::Ref<Toolchain>> m_toolchains { };

        static SG_NOINLINE SGCore::Ref<Toolchains> getInstance() noexcept;

    private:
        static inline SGCore::Ref<Toolchains> m_instance = SGCore::MakeRef<Toolchains>();
    };
}

#endif //SUNGEARENGINEEDITOR_TOOLCHAINS_H
