//
// Created by stuka on 11.05.2023.
//

#pragma once

#ifndef NATIVECORE_FILEUTILS_H
#define NATIVECORE_FILEUTILS_H

#include <iostream>

namespace Core::Utils
{
    class FileUtils
    {
    public:
        static std::string readFile(const std::string_view&);
        static void writeToFile(const std::string_view&, std::string&, const bool&);
    };
}

#endif //NATIVECORE_FILEUTILS_H
