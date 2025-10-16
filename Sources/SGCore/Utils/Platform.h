//
// Created by Ilya on 15.03.2024.
//

#ifndef SUNGEARENGINE_PLATFORM_H
#define SUNGEARENGINE_PLATFORM_H

#if defined(__clang__)
#define SG_PLATFORM_COMPILER_CLANG
#elif defined(__GNUC__)
#define SG_PLATFORM_COMPILER_GCC
#elif defined(_MSC_VER)
#define SG_PLATFORM_COMPILER_MSVC
#else
#error "Unsupported compiler. Supported compilers are: MSVC, GCC, Clang."
#endif

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#if defined(__linux__) && defined(__ANDROID__)
#define SG_PLATFORM_OS_ANDROID
#elif defined(__linux__)
#define SG_PLATFORM_OS_LINUX
#elif defined(_WIN32) || defined(_WIN64)
#define SG_PLATFORM_OS_WINDOWS
#elif defined(__APPLE__) && TARGET_OS_IPHONE == 1
#define SG_PLATFORM_OS_IOS
#elif defined(__APPLE__) && TARGET_OS_MAC == 1
#define SG_PLATFORM_OS_MAC_OS_X
#else
#error "Unsupported OS. Supported operating systems are: Windows, Linux, Mac OS X, Android, iOS"
#endif

#if defined(SG_PLATFORM_OS_WINDOWS) || defined(SG_PLATFORM_OS_LINUX) || defined(SG_PLATFORM_OS_MAC_OS_X)
#define SG_PLATFORM_PC
#elif defined(SG_PLATFORM_OS_ANDROID) || defined(SG_PLATFORM_OS_IOS)
#define SG_PLATFORM_MOBILE
#endif

#endif //SUNGEARENGINE_PLATFORM_H
