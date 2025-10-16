//
// Created by ilya on 19.03.24.
//
#include "IPlugin.h"
#include "SGCore/Utils/Platform.h"

#include <iostream>

#ifdef SG_PLATFORM_OS_LINUX
#include <dlfcn.h>
#endif

bool SGCore::IPlugin::operator==(const IPlugin& other) const noexcept
{
    return m_name == other.m_name && m_version == other.m_version;
}

bool SGCore::IPlugin::operator!=(const SGCore::IPlugin& other) const noexcept
{
    return !(*this == other);
}

std::filesystem::path SGCore::IPlugin::getLocalPath() const noexcept
{
    return m_path;
}

std::string SGCore::IPlugin::getName() const noexcept
{
    return m_version;
}

std::string SGCore::IPlugin::getVersion() const noexcept
{
    return m_version;
}
