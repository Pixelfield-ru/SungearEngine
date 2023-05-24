//
// Created by stuka on 09.05.2023.
//

#pragma once

#ifndef NATIVECORE_SHADERASSET_H
#define NATIVECORE_SHADERASSET_H

#include "IAsset.h"
#include "SGCore/Utils/FileUtils.h"

namespace Core::Memory::Assets
{
    class FileAsset : public IAsset
    {
    private:
        std::string m_data;

    public:
        void load(const std::string_view& path) override;

        const std::string& getData() noexcept;
    };
}

#endif //NATIVECORE_SHADERASSET_H
