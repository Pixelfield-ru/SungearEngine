#ifndef SUNGEARENGINE_UTILS_H
#define SUNGEARENGINE_UTILS_H

#define SG_INSTANCEOF(data, type) SGCore::Utils::instanceof<type>(data)

#define SG_MAY_NORETURN __declspec(noreturn)

#define SG_CTOR(cls) cls() = default;
#define SG_COPY_CTOR(cls) cls(const cls&) = default;
#define SG_MOVE_CTOR(cls) cls(cls&& cls) noexcept = default;

#define SG_NO_CTOR(cls) cls() = delete;
#define SG_NO_COPY(cls) cls(const cls&) = delete;
#define SG_NO_MOVE(cls) cls(cls&& cls) noexcept = delete;

#define SG_CURRENT_LOCATION_STR SGCore::Utils::sourceLocationToString(std::source_location::current())

#define SG_STRINGIFY(n) #n

#ifdef _MSC_VER
#define SG_NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
#define SG_NOINLINE __attribute__((noinline))
#endif

#ifdef _MSC_VER
#define SG_FORCEINLINE __forceinline
#elif defined(__GNUC__)
#define SG_FORCEINLINE __attribute__((always_inline))
#endif

#ifdef _MSC_VER
#define SG_CDECL __cdecl
#elif defined(__GNUC__)
#define SG_CDECL __attribute__((cdecl))
#endif

#define SG_NOMANGLING extern "C"

#ifdef _MSC_VER
#define SG_DLEXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define SG_DLEXPORT __attribute__((visibility("default")))
#endif

#ifdef _MSC_VER
#define SG_DLIMPORT __declspec(dllimport)
#elif defined(__GNUC__)
#define SG_DLIMPORT
#endif

#include <SGCore/pch.h>

#include <codecvt>
#include <locale>

#include "TypeTraits.h"
#include "SGCore/CrashHandler/Platform.h"

namespace SGCore
{
    template<typename T, std::size_t N>
    static std::array<T, N> makeFilledArray(const T& value)
    {
        std::array<T, N> arr;
        arr.fill(value); // Используем метод fill()
        return arr;
    }

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
    requires(std::is_pointer_v<T>)
    inline static size_t hashObject(T ptr) noexcept
    {
        return std::hash<const char*>()(static_cast<const char*>(reinterpret_cast<const void*>(ptr)));
    }

    template<auto FuncPtr>
    inline static size_t hashConstexprObject() noexcept
    {
        return ConstexprObject<FuncPtr>::id();
    }

    template<auto FuncPtr>
    requires(std::is_member_function_pointer_v<decltype(FuncPtr)>)
    inline static size_t hashMemberFunction(
            const typename class_function_traits<remove_noexcept_t<decltype(FuncPtr)>>::instance_type& obj) noexcept
    {
        return hashConstexprObject<FuncPtr>() ^ hashObject(&obj);
    }

    /**
     * Output of this hash will be always identical on all platforms and compilers if you are using the same string and salt.
     * @param s
     * @param salt
     * @return
     */
    static size_t hashString(const char* str, unsigned salt = 0)
    {
        unsigned h = salt;
        while (*str)
            h = h * 101 + (unsigned) *str++;
        return h;
    }

    template<typename T>
    static T fromString(const std::string& str) noexcept
    {
        if constexpr(std::is_convertible_v<std::string, T>)
        {
            return str;
        }
        else if constexpr(std::numeric_limits<T>::is_integer && std::numeric_limits<T>::is_signed)
        {
            return std::stoll(str);
        }
        else if constexpr(std::numeric_limits<T>::is_integer && !std::numeric_limits<T>::is_signed)
        {
            return std::stoull(str);
        }
        else if constexpr(std::is_floating_point_v<T>)
        {
            return std::stold(str);
        }
        else if constexpr(std::is_enum_v<T>)
        {
            return (T) fromString<std::underlying_type_t<T>>(str);
        }
        else
        {
            static_assert(always_false<T>::value, "std::string can not be casted to KeyT in std::unordered_map.");
        }
    }

    /**
     * Output of this hash will be always identical on all platforms and compilers if you are using the same string.
     * @param s
     * @param salt
     * @return
     */
    static size_t hashString(const std::string& str, unsigned salt = 0)
    {
        return hashString(str.data());
    }

    template<typename T>
    void exclude(std::vector<T>& vec, std::vector<T>& from)
    {
        std::unordered_multiset<T> st {};
        st.insert(vec.begin(), vec.end());
        st.insert(from.begin(), from.end());
        auto predicate = [&st](const T& k)
        { return st.count(k) > 1; };
        // a.erase(std::remove_if(a.begin(), a.end(), predicate), a.end());
        from.erase(std::remove_if(from.begin(), from.end(), predicate), from.end());
    }

    struct Utils
    {
        // stores type of T
        template<typename T>
        struct TypeWrapper
        {
            using type = T;
        };

        template<typename T>
        struct WeakCompare
        {
            bool operator()(const std::weak_ptr<T>& lhs, const std::weak_ptr<T>& rhs) const
            {
                auto lptr = lhs.lock(), rptr = rhs.lock();
                if (!rptr) return false; // nothing after expired pointer
                if (!lptr) return true;  // every not expired after expired pointer

                return lptr.get() < rptr.get();
            }
        };

        template<typename T>
        struct RawPointerHash
        {
            virtual size_t operator()(const T* ptr) const noexcept
            {
                return std::hash<T*>()(ptr);
            }
        };

        /**
         * Hashes result of locked weak pointer get() function.
         * @tparam T
         */
        template<typename T>
        struct WeakPointerHash
        {
            size_t operator()(const std::weak_ptr<T>& ptr) const noexcept
            {
                auto locked = ptr.lock();
                if (!locked) return 0;

                return std::hash<T*>()(locked.get());
            }
        };

        static std::string getTimeAsString(const std::string& format = "%Y-%m-%d") noexcept
        {
            const auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);

            std::ostringstream timeStringStream;
            timeStringStream << std::put_time(std::localtime(&in_time_t), format.c_str());

            return timeStringStream.str();
        }

        template<typename T>
        static std::string getTimeAsString(const std::string& format, const T& timePoint) noexcept
        {
            auto in_time_t = std::chrono::system_clock::to_time_t(timePoint);

            std::ostringstream timeStringStream;
            timeStringStream << std::put_time(std::localtime(&in_time_t), format.c_str());

            return timeStringStream.str();
        }

        [[nodiscard]] static std::chrono::sys_time<std::chrono::seconds> getStringAsTime(const std::string& dateTime, const std::string& format = "%Y-%m-%d") noexcept
        {
            #if defined(PLATFORM_OS_WINDOWS)
            std::istringstream in { dateTime };
            std::chrono::sys_time<std::chrono::seconds> dt;

            in >> std::chrono::parse(format, dt);

            return dt;
            #else
                #warning getStringAsTime DOES NOT SUPPORTED IN LINUX AND MAC OS NOW. FOR DEVS: PLEASE, IMPLEMENT getStringAsTime!
            
            return {};
            #endif
            /*std::istringstream ss{ dateTime };
            std::tm dt;
            ss >> std::get_time(&dt, format.c_str());
            return std::mktime(&dt);*/
        }

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
                                                                    const std::chrono::month& month) noexcept
        {
            return (year /
                    month /
                    std::chrono::last).day();
        }

        /**
         * Example
         * forTypes<InTypes...>([](auto t) { using type = typename decltype(t)::type; });
         * @tparam InTypes Types through which iteration will occur.
         * @tparam Func Type of function.
         * @param f Function.
         */
        template<typename... InTypes, typename Func>
        static void forTypes(Func&& func)
        {
            (func(TypeWrapper<InTypes>()), ...);
        }

        template<typename Base, typename T>
        static bool instanceof(const T* ptr)
        {
            return dynamic_cast<const Base*>(ptr) != nullptr;
        }

        template<typename CharT>
        static void
        splitString(const std::basic_string<CharT>& str, char delim,
                    std::vector<std::basic_string<CharT>>& words) noexcept
        {
            std::basic_stringstream<CharT> ss(str);
            std::basic_string<CharT> word;
            while (std::getline(ss, word, delim))
            {
                words.push_back(word);
            }
        }

        template<typename CharT>
        static std::basic_string<CharT> replaceAll(const std::basic_string<CharT>& str,
                                                   const std::basic_string<CharT>& from,
                                                   const std::basic_string<CharT>& to) noexcept
        {
            std::basic_string<CharT> resString = str;

            size_t start_pos = 0;
            while ((start_pos = resString.find(from, start_pos)) != std::basic_string<CharT>::npos)
            {
                resString.replace(start_pos, from.length(), to);
                start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
            }

            return resString;
        }

        template<typename CharT>
        static std::basic_string<CharT> replaceFirst(const std::basic_string<CharT>& str,
                                                     const std::basic_string<CharT>& from,
                                                     const std::basic_string<CharT>& to) noexcept
        {
            std::string resString = str;

            size_t start_pos = 0;
            start_pos = resString.find(from, start_pos);
            if (start_pos != std::basic_string<CharT>::npos)
            {
                resString.replace(start_pos, from.length(), to);
            }

            return resString;
        }

        template<typename CharT>
        static std::basic_string<CharT> trim(const std::basic_string<CharT>& str,
                                             const std::basic_string<CharT>& whitespace = " \t")
        {
            const auto strBegin = str.find_first_not_of(whitespace);
            if (strBegin == std::string::npos)
            {
                return ""; // no content
            }
            const auto strEnd = str.find_last_not_of(whitespace);
            const auto strRange = strEnd - strBegin + 1;
            return str.substr(strBegin, strRange);
        }

        template<typename CharT>
        static std::basic_string<CharT> reduce(const std::basic_string<CharT>& str,
                                               const std::basic_string<CharT>& fill = " ",
                                               const std::basic_string<CharT>& whitespace = " \t")
        {
            // trim first
            auto result = trim(str, whitespace);
            // replace sub ranges
            auto beginSpace = result.find_first_of(whitespace);
            while (beginSpace != std::string::npos)
            {
                const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
                const auto range = endSpace - beginSpace;
                result.replace(beginSpace, range, fill);
                const auto newStart = beginSpace + fill.length();
                beginSpace = result.find_first_of(whitespace, newStart);
            }
            return result;
        }

        template<typename CharT>
        static std::basic_string<CharT> toString(const std::vector<std::basic_string<CharT>>& vec) noexcept
        {
            std::basic_string<CharT> str;

            for (auto& s: vec)
            {
                str += s;
            }

            return str;
        }

        static std::string
        toString(const std::vector<std::string>::iterator& begin,
                 const std::vector<std::string>::iterator& end) noexcept
        {
            std::string str;

            auto it = begin;

            while (it != end)
            {
                str += *it;
                ++it;
            }

            return str;
        }

        static std::string sourceLocationToString(const std::source_location& location) noexcept
        {
            return "\tFile: " + std::string(location.file_name()) + "\n"
                                                                    "\tFunction: " +
                   std::string(location.function_name()) + "\n" + "\tLine: " +
                   std::to_string(location.line()) + "\n" + "\tColumn: " +
                   std::to_string(location.column()) + "\n";
        }

        template<typename T>
        static std::string toUTF8(const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& source)
        {
            if constexpr(std::is_same_v<T, char>)
            {
                return source;
            }
            
            std::string result;
            std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
            result = convertor.to_bytes(source);
            
            return result;
        }

        template<typename T>
        static void fromUTF8(const std::string& source, std::basic_string<T, std::char_traits<T>, std::allocator<T>>& result)
        {
            if constexpr(std::is_same_v<T, char>)
            {
                result = source;
                return;
            }
            
            std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
            result = convertor.from_bytes(source);
        }

        template<typename T>
        static std::basic_string<T, std::char_traits<T>, std::allocator<T>> fromUTF8(const std::string& source)
        {
            if constexpr(std::is_same_v<T, char>)
            {
                return source;
            }
            
            std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
            return convertor.from_bytes(source);
        }

        static long long getTimeMilliseconds() noexcept;

        static long long getTimeMicros() noexcept;

        static long long getTimeNanos() noexcept;

        static void swapEndian(unsigned char* sourceBuffer, const size_t& bufferSize) noexcept
        {
            for (size_t i = 0; i < bufferSize; ++i)
            {
                sourceBuffer[i] = sourceBuffer[bufferSize - i - 1];
            }
        }

        static void swapEndian(char* sourceBuffer, const size_t& bufferSize) noexcept
        {
            for (size_t i = 0; i < bufferSize; ++i)
            {
                sourceBuffer[i] = sourceBuffer[bufferSize - i - 1];
            }
        }

        static std::string getRealPath(const std::string& path) noexcept;

        static bool isSubpath(const std::filesystem::path& path,
                              const std::filesystem::path& base) noexcept;

        static std::filesystem::path normalizePath(const std::filesystem::path& path) noexcept;

        template<typename CharT>
        static std::string::size_type findInString(const std::basic_string<CharT>& str,
                                                   const std::basic_string<CharT>& substr,
                                                   bool caseInsensitive = false) noexcept
        {
            if (!caseInsensitive)
            {
                return str.find(substr);
            }

            auto it = std::search(
                    str.begin(), str.end(),
                    substr.begin(), substr.end(),
                    [](const CharT& ch1, const CharT& ch2)
                    {
                        if constexpr (std::is_same_v<CharT, char>)
                        {
                            return std::toupper(ch1) == std::toupper(ch2);
                        } else
                        {
                            return std::towupper(ch1) == std::towupper(ch2);
                        }
                    }
            );

            if (it != str.end())
            {
                return it - str.begin();
            }

            return std::string::npos;
        }

        static std::string consoleExecute(const std::string& cmd, std::filesystem::path* outputFile = nullptr);
    };
}


#endif //SUNGEARENGINE_UTILS_H
