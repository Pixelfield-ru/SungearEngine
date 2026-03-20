//
// Created by stuka on 11.05.2023.
//

#pragma once

#include <fstream>

#include "SGCore/Logger/Logger.h"

namespace SGCore::FileUtils
{
    SGCORE_EXPORT std::string readFile(const std::filesystem::path& path);
    /**
     * ALLOCATES BUFFER. YOU MUST DEALLOCATE IT.
     * @param path
     * @param outSize
     * @return
     */
    [[nodiscard]] SGCORE_EXPORT char* readBytes(const std::filesystem::path& path, size_t& outSize) noexcept;
    [[nodiscard]] SGCORE_EXPORT std::vector<char> readBytesBlock(const std::filesystem::path& path, const std::streamsize& offset, const std::streamsize& size) noexcept;
    /**
    * ALLOCATES BUFFER. YOU MUST DEALLOCATE IT.
    */
    [[nodiscard]] SGCORE_EXPORT char* readBytesBlockUnmanaged(const std::filesystem::path& path, const std::streamsize& offset, const std::streamsize& size) noexcept;
    SGCORE_EXPORT void writeBytes(const std::filesystem::path& path, const std::streamsize& offset, char* buffer, const std::streamsize& bufferSize, bool append);

    template<typename T>
    void writeBytes(const std::filesystem::path& path, const std::streamsize& offset, const std::vector<T>& buffer, bool append)
    {
        std::ofstream stream(path, (append ? std::ios::app : std::ios::trunc) | std::ios::binary);

        if(!stream)
        {
            LOG_E(SGCORE_TAG, "Write bytes to file error: File does not exist. Path: {}", Utils::toUTF8(path.u16string()));
            return;
        }

        stream.seekp(offset);

        stream.write(reinterpret_cast<const char*>(buffer.data()), std::ssize(buffer) * sizeof(T));
    }

    template<typename T>
    void writeBytes(const std::filesystem::path& path, const std::streamsize& offset, const T& object, bool append)
    {
        std::ofstream stream(path, (append ? std::ios::app : std::ios::trunc) | std::ios::binary);

        if(!stream)
        {
            LOG_E(SGCORE_TAG, "Write bytes to file error: File does not exist. Path: {}", Utils::toUTF8(path.u16string()));
            return;
        }

        stream.seekp(offset);

        stream.write(reinterpret_cast<const char*>(&object), sizeof(T));
    }

    SGCORE_EXPORT void writeToFile(const std::filesystem::path& path, const std::string& text, bool append, bool createDirectories);

    SGCORE_EXPORT void clearFile(const std::filesystem::path& path) noexcept;

    SGCORE_EXPORT void createDirectory(const std::string_view& path, bool createNew) noexcept;

    SGCORE_EXPORT std::filesystem::path findFile(const std::filesystem::path& relativePath, const std::filesystem::path& fileName);

    SGCORE_EXPORT std::filesystem::path getAppPublicResourcesPath() noexcept;
    SGCORE_EXPORT std::filesystem::path getAppPrivateResourcesPath() noexcept;
}
