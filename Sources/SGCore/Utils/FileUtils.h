//
// Created by stuka on 11.05.2023.
//

#ifndef SUNGEARENGINE_FILEUTILS_H
#define SUNGEARENGINE_FILEUTILS_H

#include <iostream>

namespace SGCore::FileUtils
{
    std::string readFile(const std::string_view&);

    void writeToFile(const std::string_view& path, const std::string& text, const bool& append);
}

#endif // SUNGEARENGINE_FILEUTILS_H
