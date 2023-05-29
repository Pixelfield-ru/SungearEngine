//
// Created by stuka on 27.05.2023.
//

#include "FileSmartPointer.h"
#include "SGCore/Logging/Log.h"

Core::Utils::FileSmartPointer::FileSmartPointer(std::string path, std::string mode)
{
    m_path = std::move(path);
    m_mode = std::move(mode);

    m_filePtr = fopen(m_path.c_str(), m_mode.c_str());

    if(!m_filePtr)
    {
        SGC_ERROR("Failed to open file. (path: " + m_path + ", mode: " + m_mode + ").");
    }
}

Core::Utils::FileSmartPointer::~FileSmartPointer()
{
    if(fclose(m_filePtr) == -1)
    {
        SGC_ERROR("Failed to close file. (path: " + m_path + ", mode: " + m_mode + ").");
    }
}

FILE* Core::Utils::FileSmartPointer::get() noexcept
{
    return m_filePtr;
}
