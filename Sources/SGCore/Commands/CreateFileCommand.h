//
// Created by stuka on 13.01.2025.
//

#ifndef SUNGEARENGINE_CREATEFILECOMMAND_H
#define SUNGEARENGINE_CREATEFILECOMMAND_H

#include "ICommand.h"

#include <filesystem>
#include <fstream>

namespace SGCore
{
    struct CreateFileCommand : ICommand
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

#endif //SUNGEARENGINE_CREATEFILECOMMAND_H
