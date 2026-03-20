//
// Created by stuka on 13.01.2025.
//

#pragma once

#include "ICommand.h"

#include <filesystem>
#include <fstream>

namespace SGCore
{
    struct SGCORE_EXPORT CreateFileCommand : ICommand
    {
        std::filesystem::path m_filePath;
        std::string m_fileContent;

        void execute() override
        {
            if(!m_filePath.parent_path().empty() && !std::filesystem::exists(m_filePath.parent_path()))
            {
                std::filesystem::create_directories(m_filePath.parent_path());
            }

            std::ofstream stream(m_filePath);
            stream << m_fileContent;
        }

        void undo() override
        {
            std::filesystem::remove(m_filePath);
        }
    };
}