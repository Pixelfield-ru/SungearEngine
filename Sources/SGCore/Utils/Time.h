//
// Created by stuka on 18.10.2025.
//

#pragma once

// todo: impl time
namespace SGCore::Utils
{
    long long getTimeMilliseconds() noexcept;

    long long getTimeMicros() noexcept;

    long long getTimeNanos() noexcept;

    double getTimeSecondsAsDouble() noexcept;
}