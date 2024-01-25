//
// Created by stuka on 11.05.2023.
//
#include <fstream>

#include "FileUtils.h"
#include "spdlog/spdlog.h"

std::string SGUtils::FileUtils::readFile(const std::string_view& path)
{
    try
    {
        constexpr size_t read_size = 4096;
        auto stream = std::ifstream(path.data());
        stream.exceptions(std::ios_base::badbit);

        if(!stream)
        {
            throw std::ios_base::failure("file does not exist");
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
    catch(const std::ios_base::failure& e)
    {
        //todo:
        spdlog::error("Read file error: {0}. Path: {1}", e.what(), path);
    }

    return "";
}

void SGUtils::FileUtils::writeToFile(const std::string_view& path, const std::string& text, const bool& append)
{
    try
    {
        std::ofstream fileStream;
        fileStream.open(path.data(),  (append ? std::ios::app : std::ios::trunc) | std::ios::out | std::ios::in);

        fileStream << text;
    }
    catch(const std::ios_base::failure& e)
    {
        // todo:
        spdlog::error("Write to file error: {0}. Path: {1}", e.what(), path);
    }
}
