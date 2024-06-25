//
// Created by Ilya on 15.03.2024.
//

#ifndef SUNGEARENGINE_PLATFORM_H
#define SUNGEARENGINE_PLATFORM_H

#if defined(__clang__)
#define PLATFORM_COMPILER_CLANG
#elif defined(__GNUC__)
#define PLATFORM_COMPILER_GCC
#elif defined(_MSC_VER)
#define PLATFORM_COMPILER_MSVC
#else
#error "Unsupported compiler. Supported compilers are: MSVC, GCC, Clang."
#endif

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#if defined(__linux__) && defined(__ANDROID__)
#define PLATFORM_OS_ANDROID
#elif defined(__linux__)
#define PLATFORM_OS_LINUX
#elif defined(_WIN32) || defined(_WIN64)
#define PLATFORM_OS_WINDOWS
#elif defined(__APPLE__) && TARGET_OS_IPHONE == 1
#define PLATFORM_OS_IOS
#elif defined(__APPLE__) && TARGET_OS_MAC == 1
#define PLATFORM_OS_MAC_OS_X
#else
#error "Unsupported OS. Supported operating systems are: Windows, Linux, Mac OS X, Android, iOS"
#endif

#if defined(PLATFORM_OS_WINDOWS) && !defined(PLATFORM_COMPILER_MSVC)
// #error "Unsupported combination of compiler and operating system."
#endif

#endif //SUNGEARENGINE_PLATFORM_H
