//
// Created by stuka on 07.05.2023.
//

#pragma once

#ifndef NATIVECORE_IASSET_H
#define NATIVECORE_IASSET_H

#include <iostream>
#include <memory>

namespace Core::Memory::Assets
{
    class IAsset
    {
    public:
        virtual void load(const std::string_view& path) { };
    };
}

#endif //NATIVECORE_IASSET_H
