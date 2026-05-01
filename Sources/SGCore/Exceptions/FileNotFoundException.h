//
// Created by Ilya on 27.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_FILENOTFOUNDEXCEPTION_H
#define SUNGEARENGINEEDITOR_FILENOTFOUNDEXCEPTION_H

#include <exception>
#include <filesystem>

#include <sgcore_export.h>

namespace SGCore
{
    struct SGCORE_EXPORT FileNotFoundException : std::runtime_error
    {
        FileNotFoundException(const std::filesystem::path& filePath, const std::string& err) noexcept;

        [[nodiscard]] std::filesystem::path filePath() const noexcept;

    private:
        std::filesystem::path m_filePath;
    };
}

#endif //SUNGEARENGINEEDITOR_FILENOTFOUNDEXCEPTION_H
