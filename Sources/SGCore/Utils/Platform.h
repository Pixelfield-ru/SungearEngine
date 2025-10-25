//
// Created by Ilya on 15.03.2024.
//

#pragma once

#define SG_PLATFORM_COMPILER_CLANG 0
#define SG_PLATFORM_COMPILER_GCC 0
#define SG_PLATFORM_COMPILER_MSVC 0

#define SG_PLATFORM_OS_ANDROID 0
#define SG_PLATFORM_OS_LINUX 0
#define SG_PLATFORM_OS_WINDOWS 0
#define SG_PLATFORM_OS_IOS 0
#define SG_PLATFORM_OS_MAC_OS_X 0

#define SG_PLATFORM_PC 0
#define SG_PLATFORM_MOBILE 0

#if defined(__clang__)
#undef SG_PLATFORM_COMPILER_CLANG
#define SG_PLATFORM_COMPILER_CLANG 1
#elif defined(__GNUC__)
#undef SG_PLATFORM_COMPILER_GCC
#define SG_PLATFORM_COMPILER_GCC 1
#elif defined(_MSC_VER)
#undef SG_PLATFORM_COMPILER_MSVC
#define SG_PLATFORM_COMPILER_MSVC 1
#else
#error "Unsupported compiler. Supported compilers are: MSVC, GCC, Clang."
#endif

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#if defined(__linux__) && defined(__ANDROID__)
#undef SG_PLATFORM_OS_ANDROID
#define SG_PLATFORM_OS_ANDROID 1
#elif defined(__linux__)
#undef SG_PLATFORM_OS_LINUX
#define SG_PLATFORM_OS_LINUX 1
#elif defined(_WIN32) || defined(_WIN64)
#undef SG_PLATFORM_OS_WINDOWS
#define SG_PLATFORM_OS_WINDOWS 1
#elif defined(__APPLE__) && TARGET_OS_IPHONE == 1
#undef SG_PLATFORM_OS_IOS
#define SG_PLATFORM_OS_IOS 1
#elif defined(__APPLE__) && TARGET_OS_MAC == 1
#undef SG_PLATFORM_OS_MAC_OS_X
#define SG_PLATFORM_OS_MAC_OS_X 1
#else
#error "Unsupported OS. Supported operating systems are: Windows, Linux, Mac OS X, Android, iOS"
#endif

#if SG_PLATFORM_OS_WINDOWS || SG_PLATFORM_OS_LINUX || SG_PLATFORM_OS_MAC_OS_X
#undef SG_PLATFORM_PC
#define SG_PLATFORM_PC 1
#elif SG_PLATFORM_OS_ANDROID || SG_PLATFORM_OS_IOS
#undef SG_PLATFORM_MOBILE
#define SG_PLATFORM_MOBILE 1
#endif//SUNGEARENGINE_PLATFORM_H
