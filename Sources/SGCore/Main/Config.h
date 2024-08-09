//
// Created by ilya on 05.05.24.
//

#ifndef SUNGEARENGINE_CONFIG_H
#define SUNGEARENGINE_CONFIG_H

#include <string>

namespace SGCore
{
    struct Config
    {
        std::string m_cmakePath;
        
        void save(const std::string& path) const noexcept;
        void load(const std::string& path) const noexcept;
    };
}

#endif // SUNGEARENGINE_CONFIG_H
