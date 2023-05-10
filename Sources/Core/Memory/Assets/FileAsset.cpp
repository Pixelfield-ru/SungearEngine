//
// Created by stuka on 09.05.2023.
//

#include <fstream>

#include "FileAsset.h"
#include "../../Logging/Log.h"

void Core::Memory::Assets::FileAsset::load(const std::string_view& path)
{
    this->m_data = Core::Utils::FileUtils::readFile(path);
}

const std::string& Core::Memory::Assets::FileAsset::getData() noexcept
{
    return m_data;
}