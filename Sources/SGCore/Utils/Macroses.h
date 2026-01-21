#pragma once

/*
* MisterElect (MisterChoose)
* 02.10.2024
*/

#define move_operator(T) T& operator=(T&& other) noexcept
#define move_constructor(T) T(T&& other) noexcept
#define copy_operator(T) T& operator=(const T& other) noexcept
#define copy_constructor(T) T(const T& other) noexcept

#define SG_INSTANCEOF(data, type) SGCore::Utils::instanceof<type>(data)

#define SG_MAY_NORETURN __declspec(noreturn)

#define SG_CTOR(cls) cls() = default;
#define SG_COPY_CTOR(cls) cls(const cls&) = default;
#define SG_MOVE_CTOR(cls) cls(cls&& cls) noexcept = default;

#define SG_NO_CTOR(cls) cls() = delete;
#define SG_NO_COPY(cls) cls(const cls&) = delete;
#define SG_NO_MOVE(cls) cls(cls&& cls) noexcept = delete;

#define SG_CURRENT_LOCATION_STR SGCore::Utils::sourceLocationToString(std::source_location::current())

#define SG_STRINGIFY_MACRO(n) SG_STRINGIFY(n)
#define SG_STRINGIFY(n) #n

#ifdef _MSC_VER
#define SG_NOINLINE __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
#define SG_NOINLINE __attribute__((noinline))
#endif

#ifdef _MSC_VER
#define SG_FORCEINLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define SG_FORCEINLINE __attribute__((always_inline))
#endif

#ifdef _MSC_VER
#define SG_CDECL __cdecl
#elif defined(__GNUC__) || defined(__clang__)
#define SG_CDECL __attribute__((cdecl))
#endif

#define SG_NOMANGLING extern "C"

#ifdef _MSC_VER
#define SG_DLEXPORT __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
#define SG_DLEXPORT __attribute__((visibility("default")))
#endif

#ifdef _MSC_VER
#define SG_DLIMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
#define SG_DLIMPORT
#endif


#define SG_ARG_COUNT(...) \
    SG_ARG_COUNT_IMPL(0, ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define SG_ARG_COUNT_IMPL(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N

#define SG_CONCAT(a, b) SG_CONCAT_IMPL(a, b)
#define SG_CONCAT_IMPL(a, b) a##b
