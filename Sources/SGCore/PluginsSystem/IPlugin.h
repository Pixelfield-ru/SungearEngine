//
// Created by ilya on 19.03.24.
//

#ifndef SUNGEARENGINE_IPLUGIN_H
#define SUNGEARENGINE_IPLUGIN_H

#include <vector>
#include <string>
#include <functional>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    enum PluginBuildType
    {
        PBT_DEBUG,
        PBT_RELEASE
    };
    
    struct IPlugin
    {
        friend struct PluginsManager;
        
        virtual ~IPlugin() = default;
        
        std::string m_name;
        std::string m_version;
        
        virtual std::string load(const std::vector<std::string>& args) = 0;
        
        bool operator==(const IPlugin& other) const noexcept;
        bool operator!=(const IPlugin& other) const noexcept;
        
        [[nodiscard]] std::string getLocalPath() const noexcept;
        
    private:
        std::string m_localPath;
    };
}

#endif // SUNGEARENGINE_IPLUGIN_H
