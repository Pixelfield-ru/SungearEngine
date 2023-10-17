//
// Created by stuka on 13.04.2023.
//

#pragma once

#ifndef NATIVECORE_UTILS_H
#define NATIVECORE_UTILS_H

#define SG_DELETE(p) { delete p; p = nullptr; }

#ifdef _WIN64

#include <windows.h>
#include <psapi.h>

namespace Core::Memory::Utils
{
    void getByteProcessMemUsage(size_t &pagefile_usage, size_t &working_set_size);
    void getKilobyteProcessMemUsage(size_t &pagefile_usage, size_t &working_set_size);
    void getMegabyteProcessMemUsage(size_t &pagefile_usage, size_t &working_set_size);
}

#endif

#endif //NATIVECORE_UTILS_H
