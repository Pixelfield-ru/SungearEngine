//
// Created by stuka on 11.05.2023.
//

#include "FileUtils.h"
#include "Utils.h"

std::string SGCore::FileUtils::readFile(const std::filesystem::path& path)
{
    constexpr size_t read_size = 4096;
    auto stream = std::ifstream(path);
    stream.exceptions(std::ios_base::badbit);
    
    if(!stream)
    {
        LOG_E(SGCORE_TAG, "Read file error: File does not exist. Path: {}", Utils::toUTF8<char16_t>(path.u16string()));
        return "";
    }
    
    std::string out;
    auto buf = std::string(read_size, '\0');
    
    while(stream.read(&buf[0], read_size))
    {
        out.append(buf, 0, stream.gcount());
    }
    
    out.append(buf, 0, stream.gcount());
    
    return out;
}

char* SGCore::FileUtils::readBytes(const std::filesystem::path& path, size_t& outSize) noexcept
{
    std::ifstream stream(path, std::ios::binary | std::ios::ate);
    stream.unsetf(std::ios::skipws);
    stream.exceptions(std::ios_base::badbit);
    
    if(!stream)
    {
        LOG_E(SGCORE_TAG, "Read file error: File does not exist. Path: {}", Utils::toUTF8(path.u16string()));
        return nullptr;
    }
    
    std::streamsize size = stream.tellg();
    stream.seekg(0, std::ios::beg);
    
    auto* buffer = (char*) malloc(size);
    outSize = size;
    
    if (stream.read(buffer, size))
    {
    
    }
    
    return buffer;
}

std::vector<char> SGCore::FileUtils::readBytesBlock(const std::filesystem::path& path, const std::streamsize& offset, const std::streamsize& size) noexcept
{
    std::ifstream stream(path, std::ios::binary | std::ios::ate);
    stream.unsetf(std::ios::skipws);
    stream.exceptions(std::ios_base::badbit);

    if(!stream)
    {
        LOG_E(SGCORE_TAG, "Read file error: File does not exist. Path: {}", Utils::toUTF8(path.u16string()));
        return { };
    }

    stream.seekg(offset, std::ios::beg);

    std::vector<char> output;
    output.resize(size);

    if (stream.read(output.data(), size))
    {

    }

    return output;
}

char* SGCore::FileUtils::readBytesBlockUnmanaged(const std::filesystem::path& path, const std::streamsize& offset, const std::streamsize& size) noexcept
{
    std::ifstream stream(path, std::ios::binary | std::ios::ate);
    stream.unsetf(std::ios::skipws);
    stream.exceptions(std::ios_base::badbit);

    if(!stream)
    {
        LOG_E(SGCORE_TAG, "Read file error: File does not exist. Path: {}", Utils::toUTF8(path.u16string()));
        return { };
    }

    stream.seekg(offset, std::ios::beg);

    auto* buffer = (char*) malloc(size);

    if (stream.read(buffer, size))
    {

    }

    return buffer;
}

void SGCore::FileUtils::writeBytes(const std::filesystem::path& path,
                                   const std::streamsize& offset,
                                   char* buffer,
                                   const std::streamsize& bufferSize,
                                   bool append)
{
    std::ofstream stream(path, (append ? std::ios::app : std::ios::trunc) | std::ios::binary);

    if(!stream)
    {
        LOG_E(SGCORE_TAG, "Write bytes to file error: File does not exist. Path: {}", Utils::toUTF8(path.u16string()));
        return;
    }

    stream.seekp(offset);

    stream.write(buffer, bufferSize);
}

void SGCore::FileUtils::writeToFile(const std::filesystem::path& path, const std::string& text, bool append, bool createDirectories)
{
    if(createDirectories)
    {
        if(!path.parent_path().empty())
        {
            std::filesystem::create_directories(path.parent_path().string());
        }
    }

    try
    {
        std::ofstream fileStream;
        fileStream.open(path, (append ? std::ios::app : std::ios::trunc) | std::ios::out | std::ios::in);

        fileStream << text;
    }
    catch(const std::ios_base::failure& e)
    {
        LOG_E(SGCORE_TAG, "Write to file error: {}. Path: {}", e.what(), Utils::toUTF8<char16_t>(path.u16string()));
    }
}

void SGCore::FileUtils::clearFile(const std::filesystem::path& path) noexcept
{
    std::ofstream ofs;
    ofs.open(path, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

void SGCore::FileUtils::createDirectory(const std::string_view& path, bool createNew) noexcept
{
    if(std::filesystem::exists(path) && createNew) return;

    std::filesystem::create_directories(path);
}

std::filesystem::path
SGCore::FileUtils::findFile(const std::filesystem::path& relativePath, const std::filesystem::path& fileName)
{
    for (const auto& entry: std::filesystem::recursive_directory_iterator(relativePath,
                                                                          std::filesystem::directory_options::skip_permission_denied |
                                                                          std::filesystem::directory_options::follow_directory_symlink))
    {
        if(entry.is_regular_file() && entry.path().filename() == fileName)
        {
            return entry.path();
        }
    }

    return "";
}
