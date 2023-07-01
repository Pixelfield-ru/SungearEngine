#include <fstream>

#include "FileAsset.h"
#include "SGCore/Logging/Log.h"

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::FileAsset::load(const std::string& path)
{
    m_data = Core::Utils::FileUtils::readFile(path);
    m_path = path;

    return shared_from_this();
}

std::string Core::Memory::Assets::FileAsset::getData() const noexcept
{
    return m_data;
}