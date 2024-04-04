//
// Created by ilya on 03.04.24.
//

#ifndef THREADINGAPI_UTILS_H
#define THREADINGAPI_UTILS_H

#include <chrono>

namespace SGCore
{
    static std::chrono::high_resolution_clock::time_point now() noexcept
    {
        return std::chrono::high_resolution_clock::now();
    }
    
    template<typename ScalarT, typename TimeValT>
    static ScalarT timeDiff(const std::chrono::high_resolution_clock::time_point& begin,
                            const std::chrono::high_resolution_clock::time_point& end) noexcept
    {
        std::chrono::duration<ScalarT, TimeValT> timeSpan = end - begin;
        
        return timeSpan.count();
    }
    
    template<typename T>
    requires(std::is_pointer_v<T> || std::is_member_function_pointer_v<T>)
    inline static size_t hashPointer(T ptr) noexcept
    {
        return std::hash<const char*>()(static_cast<const char*>(reinterpret_cast<const void*>(ptr)));
    }
}

#endif //THREADINGAPI_UTILS_H
