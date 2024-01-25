//
// Created by stuka on 27.05.2023.
//

#ifndef SUNGEARENGINE_FILESMARTPOINTER_H
#define SUNGEARENGINE_FILESMARTPOINTER_H

#include <iostream>

namespace SGUtils
{
    class FileSmartPointer
    {
    private:
        FILE* m_filePtr = nullptr;
        std::string m_path;
        std::string m_mode;

    public:
        FileSmartPointer(std::string, std::string);
        ~FileSmartPointer();

        FILE* get() noexcept;
    };
}

#endif //SUNGEARENGINE_FILESMARTPOINTER_H
