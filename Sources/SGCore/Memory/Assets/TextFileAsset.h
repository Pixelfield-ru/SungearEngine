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
    struct TextFileAsset : public IAsset
    {
        [[nodiscard]] std::string getData() const noexcept;
    
    protected:
        void doLoad(const std::string& path) override;
        
    private:
        std::string m_data;
    };
}

#endif //NATIVECORE_SHADERASSET_H
