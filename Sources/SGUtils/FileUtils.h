//
// Created by stuka on 11.05.2023.
//

#ifndef SUNGEARENGINE_FILEUTILS_H
#define SUNGEARENGINE_FILEUTILS_H

#include <SGUtils/pch.h>

namespace SGUtils::FileUtils
{
    std::string readFile(const std::string_view&);
    char* readBytes(const std::string_view& path, size_t& outSize) noexcept;

    void writeToFile(const std::string_view& path, const std::string& text, bool append, bool createDirectories);

    void createDirectory(const std::string_view& path, bool createNew) noexcept;
}

#endif // SUNGEARENGINE_FILEUTILS_H
