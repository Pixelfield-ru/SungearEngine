//
// Created by stuka on 09.05.2023.
//

#pragma once

#ifndef NATIVECORE_SHADERASSET_H
#define NATIVECORE_SHADERASSET_H

#include "IAsset.h"
#include "SGUtils/FileUtils.h"

namespace SGCore
{
    class TextFileAsset : public IAsset
    {
    private:
        std::string m_data;

    public:
        void load(const std::string& path) override;

        [[nodiscard]] std::string getData() const noexcept;
    };
}

#endif //NATIVECORE_SHADERASSET_H
