#include "Utils.h"

std::string SGUtils::Utils::getRealPath(const std::string& path) noexcept
{
    #if defined(PLATFORM_OS_WINDOWS)
    char buf[1024];
    char* res = _fullpath(buf, path.c_str(), 1024);
    if(res)
    {
        return buf;
    }
    return "";
    #elif defined(PLATFORM_OS_LINUX)
    char buf[PATH_MAX];
    char* res = realpath(path.c_str(), buf);
    if(res)
    {
        return buf;
    }
    return "";
    #endif
    return "";
}
