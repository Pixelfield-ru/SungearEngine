//
// Created by stuka on 18.10.2025.
//

#pragma once

#include <chrono>
#include <string>

// todo: impl time
namespace SGCore::Utils
{
    long long getTimeMilliseconds() noexcept;

    long long getTimeMicros() noexcept;

    long long getTimeNanos() noexcept;

    double getTimeSecondsAsDouble() noexcept;

    std::string getTimeAsString(const std::string& format = "%Y-%m-%d") noexcept;

    template<typename T>
    static std::string getTimeAsString(const std::string& format, const T& timePoint) noexcept
    {
        auto in_time_t = std::chrono::system_clock::to_time_t(timePoint);

        std::ostringstream timeStringStream;
        timeStringStream << std::put_time(std::localtime(&in_time_t), format.c_str());

        return timeStringStream.str();
    }

    [[nodiscard]] static std::chrono::sys_time<std::chrono::seconds> getStringAsTime(const std::string& dateTime, const std::string& format = "%Y-%m-%d") noexcept;

    template<typename T>
        [[nodiscard]] static std::chrono::year getYearFromTimePoint(const T& timePoint) noexcept
    {
        const auto d0 = std::chrono::floor<std::chrono::days>(timePoint);
        const std::chrono::year_month_day ymd = d0;
        return ymd.year();
    }

    template<typename T>
    [[nodiscard]] static std::chrono::month getMonthFromTimePoint(const T& timePoint) noexcept
    {
        const auto d0 = std::chrono::floor<std::chrono::days>(timePoint);
        const std::chrono::year_month_day ymd = d0;
        return ymd.month();
    }

    template<typename T>
    [[nodiscard]] static std::chrono::day getDayFromTimePoint(const T& timePoint) noexcept
    {
        const auto d0 = std::chrono::floor<std::chrono::days>(timePoint);
        const std::chrono::year_month_day ymd = d0;
        return ymd.day();
    }

    template<typename T>
    [[nodiscard]] static std::chrono::day getLastDayOfTimePoint(const T& timePoint) noexcept
    {
        return (SGCore::Utils::getYearFromTimePoint(timePoint) /
                SGCore::Utils::getMonthFromTimePoint(timePoint) /
                std::chrono::last).day();
    }

    [[nodiscard]] static std::chrono::day getLastDayOfTimePoint(const std::chrono::year& year,
                                                                const std::chrono::month& month) noexcept;
}