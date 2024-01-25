//
// Created by stuka on 27.05.2023.
//

#include "FileSmartPointer.h"
#include <spdlog/spdlog.h>

SGUtils::FileSmartPointer::FileSmartPointer(std::string path, std::string mode)
{
    m_path = std::move(path);
    m_mode = std::move(mode);

    m_filePtr = fopen(m_path.c_str(), m_mode.c_str());

    if(!m_filePtr)
    {
        //todo:
        spdlog::error("Failed to open file. (path: {0}, mode: {1})", m_path, m_mode);
    }
}

SGUtils::FileSmartPointer::~FileSmartPointer()
{
    if(fclose(m_filePtr) == -1)
    {
        // todo:
        spdlog::error("Failed to close file. (path: {0}, mode: {1})", m_path, m_mode);
    }
}

FILE* SGUtils::FileSmartPointer::get() noexcept
{
    return m_filePtr;
}
