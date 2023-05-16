//
// Created by stuka on 11.05.2023.
//
#include <fstream>

#include "FileUtils.h"
#include "SGCore/Logging/Log.h"

std::string Core::Utils::FileUtils::readFile(const std::string_view& path)
{
    try
    {
        constexpr auto read_size = std::size_t(4096);
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
        Logging::consolePrintf(Logging::MessageType::SG_ERROR, "FILE_ASSET_LOAD: error: " + std::string(e.what()) + ". Path: " + path.data());
    }

    return "";
}