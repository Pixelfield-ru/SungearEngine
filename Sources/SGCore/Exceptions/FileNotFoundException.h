//
// Created by Ilya on 27.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_FILENOTFOUNDEXCEPTION_H
#define SUNGEARENGINEEDITOR_FILENOTFOUNDEXCEPTION_H

#include <exception>
#include <filesystem>

namespace SGCore
{
    struct FileNotFoundException : std::exception
    {
        FileNotFoundException(const std::filesystem::path& filePath, const std::string& err) noexcept;

        [[nodiscard]] std::filesystem::path filePath() const noexcept;

    private:
        std::filesystem::path m_filePath;
    };
}

#endif //SUNGEARENGINEEDITOR_FILENOTFOUNDEXCEPTION_H
