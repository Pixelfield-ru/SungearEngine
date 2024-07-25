//
// Created by Ilya on 17.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_TOOLCHAINS_H
#define SUNGEARENGINEEDITOR_TOOLCHAINS_H

#include <vector>
#include <SGCore/Utils/Utils.h>
#include <SGCore/Utils/UniqueNamesManager.h>
#include <SGCore/Main/CoreGlobals.h>
#include "Toolchain.h"

namespace SGE
{
    struct Toolchains
    {
        static SG_NOINLINE SGCore::Ref<Toolchains> getInstance() noexcept;

        void addToolchain(const SGCore::Ref<Toolchain>& toolchain) noexcept;
        void removeToolchain(const std::string& name) noexcept;
        SGCore::Ref<Toolchain> getToolchain(const std::string& name) noexcept;
        void clear() noexcept;

        [[nodiscard]] const std::vector<SGCore::Ref<Toolchain>>& getToolchains() const noexcept;

    private:
        std::vector<SGCore::Ref<Toolchain>> m_toolchains;
        SGCore::Ref<SGCore::UniqueNamesManager> m_toolchainsNamesManager = SGCore::MakeRef<SGCore::UniqueNamesManager>();

        static inline SGCore::Ref<Toolchains> m_instance = SGCore::MakeRef<Toolchains>();
    };
}

#endif //SUNGEARENGINEEDITOR_TOOLCHAINS_H
