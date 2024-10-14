//
// Created by Ilya on 30.07.2024.
//

#include "EngineSettings.h"

#include <SGCore/Utils/FileUtils.h>

#include "Utils/Serializers.h"

SGCore::Ref<SGE::EngineSettings> SGE::EngineSettings::getInstance() noexcept
{
    return m_instance;
}

const std::vector<SGCore::Ref<SGE::Toolchain>>& SGE::EngineSettings::getToolchains() const noexcept
{
    return m_toolchains;
}

void SGE::EngineSettings::addToolchain(const SGCore::Ref<SGE::Toolchain>& toolchain) noexcept
{
    toolchain->m_name.attachToManager(m_toolchainsNamesManager);

    m_toolchains.push_back(toolchain);
}

void SGE::EngineSettings::removeToolchain(const std::string& name) noexcept
{
    std::erase_if(m_toolchains, [name](const SGCore::Ref<Toolchain>& toolchain) {
        return toolchain->m_name == name;
    });
}

SGCore::Ref<SGE::Toolchain> SGE::EngineSettings::getToolchain(const std::string& name) noexcept
{
    auto foundIt = std::find_if(m_toolchains.begin(), m_toolchains.end(), [&name](const SGCore::Ref<Toolchain>& toolchain) {
        return toolchain->m_name == name;
    });

    return foundIt != m_toolchains.end() ? *foundIt : nullptr;
}

void SGE::EngineSettings::clearToolchains() noexcept
{
    m_toolchains.clear();
    m_toolchainsNamesManager->clearCounters();
}

SGE::EngineSettings& SGE::EngineSettings::operator=(const SGE::EngineSettings& rhs) noexcept
{
    if(this == std::addressof(rhs)) return *this;

    *m_toolchainsNamesManager = *rhs.m_toolchainsNamesManager;
    m_toolchains.clear();

    for(const auto& t : rhs.m_toolchains)
    {
        m_toolchains.push_back(SGCore::Ref<Toolchain>(t->copy()));
    }

    return *this;
}

void SGE::EngineSettings::save(const std::filesystem::path& toPath) const noexcept
{
    const std::string savedSettings = SGCore::Serde::Serializer::toFormat(*this);
    SGCore::FileUtils::writeToFile(toPath, savedSettings, false, true);
}

void SGE::EngineSettings::load(const std::filesystem::path& fromPath) noexcept
{
    if(!std::filesystem::exists(fromPath)) return;

    const std::string settingsContent = SGCore::FileUtils::readFile(fromPath);

    std::string outputLog;
    SGCore::Serde::Serializer::fromFormat(settingsContent, *this, outputLog);

    if(!outputLog.empty())
    {
        // todo: show error dialog
    }
}
