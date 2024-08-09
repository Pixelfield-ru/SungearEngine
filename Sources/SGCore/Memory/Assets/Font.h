//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_FONT_H
#define SUNGEARENGINE_FONT_H

#include <SGCore/pch.h>

#include "IAsset.h"
#include "FontSpecialization.h"

namespace SGCore
{
    struct Font : public IAsset
    {
        // TODO:
        void serializeData(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName) override;
        // TODO:
        void serializeMeta(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName) override;

        Ref<FontSpecialization> addOrGetSpecialization(const SGCore::FontSpecializationSettings& fontSpecializationSettings);
        Ref<FontSpecialization> getSpecialization(const FontSpecializationSettings& fontSpecializationSettings);
        
        const auto& getSpecializations() const noexcept
        {
            return m_specializations;
        }
        
    protected:
        void doLoad(const std::string& path) override;
        
    private:
        std::unordered_map<FontSpecializationSettings, Ref<FontSpecialization>> m_specializations;
    };
}

#endif //SUNGEARENGINE_FONT_H
