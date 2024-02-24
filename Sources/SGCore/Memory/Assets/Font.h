//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_FONT_H
#define SUNGEARENGINE_FONT_H

#include "IAsset.h"
#include "FontSpecialization.h"

#include <freetype/freetype.h>
#include <map>
#include <unordered_set>

namespace SGCore
{
    struct Font : public IAsset
    {
        void load(const std::string& path) override;
        
        Ref<FontSpecialization> getSpecialization(const FontSpecializationSettings& fontSpecializationSettings);
        
    private:
        std::unordered_map<FontSpecializationSettings, Ref<FontSpecialization>> m_specializations;
    };
}

#endif //SUNGEARENGINE_FONT_H
