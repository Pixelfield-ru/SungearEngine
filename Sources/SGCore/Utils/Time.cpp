//
// Created by stuka on 18.10.2025.
//

#include "Time.h"

#include <sstream>
#include "Platform.h"

long long SGCore::Utils::getTimeNanos() noexcept
{
    using namespace std::chrono;
    auto timepoint = system_clock::now();
    auto fine = time_point_cast<std::chrono::nanoseconds>(timepoint).time_since_epoch().count();

    return fine;
}

long long SGCore::Utils::getTimeMicros() noexcept
{
    using namespace std::chrono;
    auto timepoint = system_clock::now();
    auto fine = time_point_cast<std::chrono::microseconds>(timepoint).time_since_epoch().count();

    return fine;
}

long long SGCore::Utils::getTimeMilliseconds() noexcept
{
    using namespace std::chrono;
    auto timepoint = system_clock::now();
    auto fine = time_point_cast<std::chrono::milliseconds>(timepoint).time_since_epoch().count();

    return fine;
}

double SGCore::Utils::getTimeSecondsAsDouble() noexcept
{
    using namespace std::chrono;
    static auto start_time = steady_clock::now();

    auto current_time = steady_clock::now();
    auto duration = duration_cast<microseconds>(current_time - start_time);
    return duration.count() / 1000000.0;
}

std::string SGCore::Utils::getTimeAsString(const std::string& format) noexcept
{
    const auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream timeStringStream;
    timeStringStream << std::put_time(std::localtime(&in_time_t), format.c_str());

    return timeStringStream.str();
}

std::chrono::sys_time<std::chrono::seconds> SGCore::Utils::getStringAsTime(const std::string& dateTime,
                                                                           const std::string& format) noexcept
{
#if SG_PLATFORM_OS_WINDOWS
    std::istringstream in { dateTime };
    std::chrono::sys_time<std::chrono::seconds> dt;

    in >> std::chrono::parse(format, dt);

    return dt;
#else
#warning getStringAsTime DOES NOT SUPPORTED IN LINUX, MAC OS AND Android FOR NOW. FOR DEVS: PLEASE, IMPLEMENT getStringAsTime!

    return {};
#endif
    /*std::istringstream ss{ dateTime };
    std::tm dt;
    ss >> std::get_time(&dt, format.c_str());
    return std::mktime(&dt);*/
}

std::chrono::day SGCore::Utils::getLastDayOfTimePoint(const std::chrono::year& year,
                                                      const std::chrono::month& month) noexcept
{
    return (year / month / std::chrono::last).day();
}

