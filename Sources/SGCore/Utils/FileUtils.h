//
// Created by stuka on 11.05.2023.
//

#ifndef SUNGEARENGINE_FILEUTILS_H
#define SUNGEARENGINE_FILEUTILS_H

#include <SGCore/pch.h>

namespace SGCore::FileUtils
{
    std::string readFile(const std::filesystem::path& path);
    char* readBytes(const std::filesystem::path& path, size_t& outSize) noexcept;

    void writeToFile(const std::filesystem::path& path, const std::string& text, bool append, bool createDirectories);

    void clearFile(const std::filesystem::path& path) noexcept;

    void createDirectory(const std::string_view& path, bool createNew) noexcept;

    std::filesystem::path findFile(const std::filesystem::path& relativePath, const std::filesystem::path& fileName);
}

#endif // SUNGEARENGINE_FILEUTILS_H
