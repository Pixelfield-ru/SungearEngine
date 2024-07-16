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
        spdlog::error("Read file error: File does not exist. Path: {0}", Utils::toUTF8<char16_t>(path.u16string()));
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

char* SGCore::FileUtils::readBytes(const std::string_view& path, size_t& outSize) noexcept
{
    std::ifstream stream(path.data(), std::ios::binary | std::ios::ate);
    stream.unsetf(std::ios::skipws);
    stream.exceptions(std::ios_base::badbit);
    
    if(!stream)
    {
        spdlog::error("Read file error: File does not exist. Path: {0}", path);
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

void SGCore::FileUtils::writeToFile(const std::filesystem::path& path, const std::string& text, bool append, bool createDirectories)
{
    if(createDirectories)
    {
        std::filesystem::path fPath(path);
        if(!fPath.parent_path().empty())
        {
            std::filesystem::create_directories(fPath.parent_path().string());
        }
    }

    try
    {
        std::ofstream fileStream;
        fileStream.open(path,  (append ? std::ios::app : std::ios::trunc) | std::ios::out | std::ios::in | std::ios::binary);

        fileStream << text;
    }
    catch(const std::ios_base::failure& e)
    {
        spdlog::error("Write to file error: {0}. Path: {1}", e.what(), Utils::toUTF8<char16_t>(path.u16string()));
    }
}

void SGCore::FileUtils::createDirectory(const std::string_view& path, bool createNew) noexcept
{
    if(std::filesystem::exists(path) && createNew) return;

    std::filesystem::create_directories(path);
}

std::filesystem::path SGCore::FileUtils::findFile(const std::filesystem::path& relativePath, const std::filesystem::path& fileName) noexcept
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(relativePath)) {
        if (entry.is_regular_file() && entry.path().filename() == fileName) {
            return entry.path();
        }
    }

    return "";
}
