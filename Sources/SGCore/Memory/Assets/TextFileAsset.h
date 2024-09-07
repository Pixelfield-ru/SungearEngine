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
        // TODO:
        void serializeData(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName) override;
        // TODO:
        void serializeMeta(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName) override;

        [[nodiscard]] std::string getData() const noexcept;
    
    protected:
        void doLoad(const std::filesystem::path& path) override;
        
    private:
        std::string m_data;
    };
}

#endif //NATIVECORE_SHADERASSET_H
