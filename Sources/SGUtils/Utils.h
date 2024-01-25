#ifndef SUNGEARENGINE_UTILS_H
#define SUNGEARENGINE_UTILS_H

#define SG_INSTANCEOF(data, type) SGUtils::Utils::instanceof<type>(data)

#define SG_MAY_NORETURN __declspec(noreturn)

#define SG_CTOR(cls) cls() = default;
#define SG_COPY_CTOR(cls) cls(const cls&) = default;
#define SG_MOVE_CTOR(cls) cls(cls&& cls) noexcept = default;

#define SG_NO_CTOR(cls) cls() = delete;
#define SG_NO_COPY(cls) cls(const cls&) = delete;
#define SG_NO_MOVE(cls) cls(cls&& cls) noexcept = delete;

#define SG_CURRENT_LOCATION_STR SGUtils::Utils::sourceLocationToString(std::source_location::current())

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <source_location>
#include <chrono>

namespace SGUtils
{
    class Utils
    {
    public:
        // stores type of T
        template<typename T>
        struct TypeWrapper { using type = T; };

        /**
         * Example
         * forTypes<InTypes...>([](auto t) { using type = typename decltype(t)::type; });
         * @tparam InTypes Types through which iteration will occur.
         * @tparam Func Type of function.
         * @param f Function.
         */
        template <typename... InTypes, typename Func>
        static void forTypes(Func&& f)
        {
            (f(TypeWrapper<InTypes>()), ...);
        }

        template<typename Base, typename T>
        static bool instanceof(const T* ptr)
        {
            return dynamic_cast<const Base*>(ptr) != nullptr;
        }

        static void splitString(const std::string& str, char delim, std::vector<std::string>& words) noexcept
        {
            std::stringstream ss(str);
            std::string word;
            while(std::getline(ss, word, delim)) {
                words.push_back(word);
            }
        }

        static std::string replaceAll(const std::string& str, const std::string& from, const std::string& to) noexcept
        {
            std::string resString = str;

            size_t start_pos = 0;
            while((start_pos = resString.find(from, start_pos)) != std::string::npos) {
                resString.replace(start_pos, from.length(), to);
                start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
            }

            return resString;
        }

        static std::string replaceFirst(const std::string& str, const std::string& from, const std::string& to) noexcept
        {
            std::string resString = str;

            size_t start_pos = 0;
            start_pos = resString.find(from, start_pos);
            if(start_pos != std::string::npos) {
                resString.replace(start_pos, from.length(), to);
            }

            return resString;
        }

        static std::string sourceLocationToString(const std::source_location& location) noexcept
        {
            return "\tFile: " + std::string(location.file_name()) + "\n"
                                                                    "\tFunction: " +
                   std::string(location.function_name()) + "\n" + "\tLine: " +
                   std::to_string(location.line()) + "\n" + "\tColumn: " +
                   std::to_string(location.column()) + "\n";
        }

        static long long getTimeMicros() noexcept
        {
            using namespace std::chrono;
            auto timepoint = system_clock::now();
            auto coarse = system_clock::to_time_t(timepoint);
            auto fine = time_point_cast<std::chrono::microseconds>(timepoint).time_since_epoch().count();

            return fine;
        }

        static long long getTimeNanos() noexcept
        {
            using namespace std::chrono;
            auto timepoint = system_clock::now();
            auto coarse = system_clock::to_time_t(timepoint);
            auto fine = time_point_cast<std::chrono::nanoseconds>(timepoint).time_since_epoch().count();

            return fine;
        }
    };
}


#endif //SUNGEARENGINE_UTILS_H
