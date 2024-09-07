//
// Created by ilya on 21.05.24.
//

#ifndef SUNGEARENGINE_SVGIMAGE_H
#define SUNGEARENGINE_SVGIMAGE_H

#include <SGCore/pch.h>

#include "IAsset.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SVGImageSpecialization.h"

namespace SGCore
{
    struct SVGImage : public IAsset
    {
        // TODO:
        void serializeData(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName) override;
        // TODO:
        void serializeMeta(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName) override;

        [[nodiscard]] Ref<SVGImageSpecialization> getSpecialization(const std::uint32_t& width, const std::uint32_t& height) noexcept;
        void removeSpecialization(const std::uint32_t& width, const std::uint32_t& height) noexcept;
    
    protected:
        void doLoad(const std::filesystem::path& path) override;
        
        std::vector<Ref<SVGImageSpecialization>> m_specializations;
        
    private:
        std::unique_ptr<lunasvg::Document> m_document;
    };
}

#endif //SUNGEARENGINE_SVGIMAGE_H
