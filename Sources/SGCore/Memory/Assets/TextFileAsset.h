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

        sg_implement_type_id(TextFileAsset, 3)

        [[nodiscard]] std::string getData() const noexcept;
    
    protected:
        void doLoad(const std::filesystem::path& path) override;

        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;
        
    private:
        std::streamsize m_dataOffsetInPackage = 0;
        std::streamsize m_dataSizeInPackage = 0;

        std::string m_data;
    };
}

#endif //NATIVECORE_SHADERASSET_H
