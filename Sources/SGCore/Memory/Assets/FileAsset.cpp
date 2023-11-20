#include <fstream>

#include "FileAsset.h"
#include "SGCore/Logging/Log.h"

std::shared_ptr<SGCore::IAsset> SGCore::FileAsset::load(const std::string& path)
{
    m_data = FileUtils::readFile(path);
    m_path = path;

    return shared_from_this();
}

std::string SGCore::FileAsset::getData() const noexcept
{
    return m_data;
}