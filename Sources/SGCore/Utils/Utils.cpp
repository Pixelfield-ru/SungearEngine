#include "Utils.h"

std::string SGCore::Utils::getRealPath(const std::string& path) noexcept
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

bool SGCore::Utils::isSubpath(const std::filesystem::path& path, const std::filesystem::path& base) noexcept
{
    // return path.compare(base) < 0;
    auto rel = std::filesystem::relative(path, base);
    return !rel.empty() && rel.native()[0] != '.';
}

long long SGCore::Utils::getTimeNanos() noexcept
{
    using namespace std::chrono;
    auto timepoint = system_clock::now();
    auto coarse = system_clock::to_time_t(timepoint);
    auto fine = time_point_cast<std::chrono::nanoseconds>(timepoint).time_since_epoch().count();

    return fine;
}

long long SGCore::Utils::getTimeMicros() noexcept
{
    using namespace std::chrono;
    auto timepoint = system_clock::now();
    auto coarse = system_clock::to_time_t(timepoint);
    auto fine = time_point_cast<std::chrono::microseconds>(timepoint).time_since_epoch().count();

    return fine;
}

long long SGCore::Utils::getTimeMilliseconds() noexcept
{
    using namespace std::chrono;
    auto timepoint = system_clock::now();
    auto coarse = system_clock::to_time_t(timepoint);
    auto fine = time_point_cast<std::chrono::milliseconds>(timepoint).time_since_epoch().count();

    return fine;
}
