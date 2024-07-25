//
// Created by Ilya on 17.07.2024.
//
#include "Toolchains.h"

SGCore::Ref<SGE::Toolchains> SGE::Toolchains::getInstance() noexcept
{
    return m_instance;
}

const std::vector<SGCore::Ref<SGE::Toolchain>>& SGE::Toolchains::getToolchains() const noexcept
{
    return m_toolchains;
}

void SGE::Toolchains::addToolchain(const SGCore::Ref<SGE::Toolchain>& toolchain) noexcept
{
    toolchain->m_name.attachToManager(m_toolchainsNamesManager);

    m_toolchains.push_back(toolchain);
}

void SGE::Toolchains::removeToolchain(const std::string& name) noexcept
{
    std::erase_if(m_toolchains, [name](const SGCore::Ref<Toolchain>& toolchain) {
       return toolchain->m_name == name;
    });
}

SGCore::Ref<SGE::Toolchain> SGE::Toolchains::getToolchain(const std::string& name) noexcept
{
    auto foundIt = std::find_if(m_toolchains.begin(), m_toolchains.end(), [&name](const SGCore::Ref<Toolchain>& toolchain) {
        return toolchain->m_name == name;
    });

    return foundIt != m_toolchains.end() ? *foundIt : nullptr;
}

void SGE::Toolchains::clear() noexcept
{
    m_toolchains.clear();
    m_toolchainsNamesManager->clearCounters();
}
