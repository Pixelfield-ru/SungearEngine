#include "Utils.h"

#ifdef _WIN64

void Core::Memory::Utils::getByteProcessMemUsage(size_t &pagefile_usage, size_t &working_set_size)
{
    PROCESS_MEMORY_COUNTERS memCounter;
    BOOL result = GetProcessMemoryInfo(GetCurrentProcess(),
                                       &memCounter,
                                       sizeof( memCounter ));

    pagefile_usage = memCounter.PagefileUsage;
    working_set_size = memCounter.WorkingSetSize;
}

void Core::Memory::Utils::getKilobyteProcessMemUsage(size_t &pagefile_usage, size_t &working_set_size)
{
    getByteProcessMemUsage(pagefile_usage, working_set_size);
    pagefile_usage /= 1024;
    working_set_size /= 1024;
}

void Core::Memory::Utils::getMegabyteProcessMemUsage(size_t &pagefile_usage, size_t &working_set_size)
{
    getByteProcessMemUsage(pagefile_usage, working_set_size);
    pagefile_usage /= 1024 * 1024;
    working_set_size /= 1024 * 1024;
}

#endif