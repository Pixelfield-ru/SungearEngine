//
// Created by stuka on 09.05.2023.
//

#include <fstream>

#include "FileAsset.h"
#include "../../Logging/Log.h"

void Core::Memory::Assets::FileAsset::load(const std::string_view& path)
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

        this->m_data = out;
    }
    catch(const std::ios_base::failure& e)
    {
        Logging::consolePrintf(Logging::MessageType::SG_ERROR, "FILE_ASSET_LOAD: error: %s", e.what());
    }
}

const std::string& Core::Memory::Assets::FileAsset::getData() noexcept
{
    return m_data;
}