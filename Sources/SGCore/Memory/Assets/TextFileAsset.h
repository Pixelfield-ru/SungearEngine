//
// Created by stuka on 09.05.2023.
//

#pragma once

#ifndef NATIVECORE_SHADERASSET_H
#define NATIVECORE_SHADERASSET_H

#include "IAsset.h"
#include "SGCore/Utils/FileUtils.h"

namespace SGCore
{
    struct TextFileAsset : public IAsset
    {
        sg_serde_as_friend()

        static inline size_t asset_type_id = StaticTypeID<TextFileAsset>::setID(3);

        [[nodiscard]] std::string getData() const noexcept;
    
    protected:
        void doLoad(const std::filesystem::path& path) override;
        
    private:
        std::string m_data;
    };
}

#endif //NATIVECORE_SHADERASSET_H
