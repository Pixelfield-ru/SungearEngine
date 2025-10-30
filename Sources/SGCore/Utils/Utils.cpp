#include "Utils.h"
#include "FileUtils.h"
#include "Unique/UUID.h"

std::string SGCore::Utils::sourceLocationToString(const std::source_location& location) noexcept
{
    return "\tFile: " + std::string(location.file_name()) + "\n"
                                                                    "\tFunction: " +
                   std::string(location.function_name()) + "\n" + "\tLine: " +
                   std::to_string(location.line()) + "\n" + "\tColumn: " +
                   std::to_string(location.column()) + "\n";
}

void SGCore::Utils::swapEndian(unsigned char* sourceBuffer, const size_t& bufferSize) noexcept
{
    for (size_t i = 0; i < bufferSize; ++i)
    {
        sourceBuffer[i] = sourceBuffer[bufferSize - i - 1];
    }
}

void SGCore::Utils::swapEndian(char* sourceBuffer, const size_t& bufferSize) noexcept
{
    for (size_t i = 0; i < bufferSize; ++i)
    {
        sourceBuffer[i] = sourceBuffer[bufferSize - i - 1];
    }
}

std::string SGCore::Utils::getRealPath(const std::string& path) noexcept
{
    #if SG_PLATFORM_OS_WINDOWS
    char buf[1024];
    char* res = _fullpath(buf, path.c_str(), 1024);
    if(res)
    {
        return buf;
    }
    return "";
    #elif SG_PLATFORM_OS_LINUX || SG_PLATFORM_OS_ANDROID
    char buf[PATH_MAX];
    char* res = realpath(path.c_str(), buf);
    if(res)
    {
        return buf;
    }
    return "";
    #else
        #error Please, implement SGCore::Utils::getRealPath function for your target OS!
    #endif
    return "";
}

bool SGCore::Utils::isSubpath(const std::filesystem::path& path, const std::filesystem::path& base) noexcept
{
    // return path.compare(base) < 0;
    auto rel = std::filesystem::relative(path, base);
    return !rel.empty() && rel.native()[0] != '.';
}

std::filesystem::path SGCore::Utils::normalizePath(const std::filesystem::path& path) noexcept
{
    LOGCAT_I(SGCORE_TAG, "Normalized path: '{}'", Utils::toUTF8(path.u16string()))

    auto finalPath = path.lexically_normal();

    finalPath = replaceAll<char>(
        Utils::toUTF8(finalPath.u16string()),
        "\\",
        "/"
    );

    return finalPath;
}

std::string SGCore::Utils::consoleExecute(const std::string& cmd, std::filesystem::path* outputFile)
{
    UUID execUUID;

    // if(std::filesystem::exists("ConsoleTmp/"))
    FileUtils::createDirectory("ConsoleTmp/Output", false);
    FileUtils::writeToFile("ConsoleTmp/" + execUUID.getUUID() + ".cmd", cmd, false, true);
    std::filesystem::path outputFilePath = "ConsoleTmp/Output/" + execUUID.getUUID() + "_output.txt";
    if(outputFile)
    {
        *outputFile = outputFilePath;
    }
    std::system(fmt::format("chcp 65001 & cd ConsoleTmp & {0}.cmd > Output/{1}_output.txt", execUUID.getUUID(), execUUID.getUUID()).c_str());
    return FileUtils::readFile(outputFilePath);

    /*std::array<char, 2048> buffer { };
    std::string result;
    #if SG_PLATFORM_OS_WINDOWS
    std::unique_ptr<FILE, void (*)(FILE*)> pipe(_popen(cmd.c_str(), "r"),
                                                [](FILE* f) -> void
                                                {
                                                    // wrapper to ignore the return value from pclose() is needed with newer versions of gnu g++
                                                    std::ignore = _pclose(f);
                                                });
    #elif SG_PLATFORM_OS_LINUX
    std::unique_ptr<FILE, void(*)(FILE*)> pipe(popen(cmd.c_str(), "r"),
    [](FILE * f) -> void
    {
        // wrapper to ignore the return value from pclose() is needed with newer versions of gnu g++
        std::ignore = pclose(f);
    });
    #endif
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    return result;*/
}

std::chrono::high_resolution_clock::time_point SGCore::now() noexcept
{
    return std::chrono::high_resolution_clock::now();
}
