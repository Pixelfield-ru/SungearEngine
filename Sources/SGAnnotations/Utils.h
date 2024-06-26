//
// Created by ilya on 26.06.24.
//

#ifndef SGANNOTATIONSUTIL_UTILS_H
#define SGANNOTATIONSUTIL_UTILS_H

#include <string>
#include <fstream>
#include <locale>
#include <codecvt>
#include <vector>

namespace SGAnnotations::Utils
{
    static std::string readFile(const std::string& path) noexcept
    {
        constexpr size_t read_size = 4096;
        auto stream = std::ifstream(path.data());
        stream.exceptions(std::ios_base::badbit);
        
        if(!stream)
        {
            std::printf("Read file error: File does not exist. Path: %s\n", path.c_str());
            return "";
        }
        
        std::string out;
        auto buf = std::string(read_size, '\0');
        
        while(stream.read(&buf[0], read_size))
        {
            out.append(buf, 0, stream.gcount());
        }
        
        out.append(buf, 0, stream.gcount());
        
        return out;
    }
    
    template<typename T>
    static std::string toUTF8(const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& source)
    {
        std::string result;
        std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
        result = convertor.to_bytes(source);
        return result;
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
}

#endif //SGANNOTATIONSUTIL_UTILS_H
