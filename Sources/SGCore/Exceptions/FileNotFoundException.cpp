//
// Created by Ilya on 27.07.2024.
//

#include "FileNotFoundException.h"

SGCore::FileNotFoundException::FileNotFoundException(const std::filesystem::path& filePath,
                                                     const std::string& err) noexcept : std::exception(err.c_str())
{
    m_filePath = filePath;
}

std::filesystem::path SGCore::FileNotFoundException::filePath() const noexcept
{
    return m_filePath;
}
