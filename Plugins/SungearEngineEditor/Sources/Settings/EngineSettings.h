//
// Created by Ilya on 30.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_ENGINESETTINGS_H
#define SUNGEARENGINEEDITOR_ENGINESETTINGS_H

#include <vector>
#include <SGCore/Utils/Utils.h>
#include <SGCore/Utils/UniqueNamesManager.h>
#include <SGCore/Main/CoreGlobals.h>
#include "Toolchains/Toolchain.h"

sg_predeclare_serdespec()

namespace SGE
{
    struct EngineSettings
    {
        sg_serdespec_as_friend()

        static SG_NOINLINE SGCore::Ref<EngineSettings> getInstance() noexcept;

        void addToolchain(const SGCore::Ref<Toolchain>& toolchain) noexcept;
        void removeToolchain(const std::string& name) noexcept;
        SGCore::Ref<Toolchain> getToolchain(const std::string& name) noexcept;
        void clearToolchains() noexcept;

        [[nodiscard]] const std::vector<SGCore::Ref<Toolchain>>& getToolchains() const noexcept;

        void save(const std::filesystem::path& toPath) const noexcept;
        void load(const std::filesystem::path& fromPath) noexcept;

        EngineSettings& operator=(const EngineSettings&) noexcept;

    private:
        std::vector<SGCore::Ref<Toolchain>> m_toolchains;
        SGCore::Ref<SGCore::UniqueNamesManager> m_toolchainsNamesManager = SGCore::MakeRef<SGCore::UniqueNamesManager>();

        static inline SGCore::Ref<EngineSettings> m_instance = SGCore::MakeRef<EngineSettings>();
    };
}

#endif //SUNGEARENGINEEDITOR_ENGINESETTINGS_H
