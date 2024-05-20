//
// Created by ilya on 23.02.24.
//

#include <spdlog/spdlog.h>

#include "Font.h"
#include "SGCore/UI/FontsManager.h"
#include "SGCore/Graphics/API/ITexture2D.h"

void SGCore::Font::doLoad(const std::string& path)
{

}

SGCore::Ref<SGCore::FontSpecialization>
SGCore::Font::addOrGetSpecialization(const SGCore::FontSpecializationSettings& fontSpecializationSettings)
{
    Ref<FontSpecialization> specialization;
    auto foundSpecIter = m_specializations.find(fontSpecializationSettings);
    if(foundSpecIter == m_specializations.end())
    {
        specialization = MakeRef<FontSpecialization>();
        specialization->m_settings = fontSpecializationSettings;
        
        specialization->prepareToBuild(m_path.string());
        
        m_specializations[fontSpecializationSettings] = specialization;
    }
    else
    {
        specialization = foundSpecIter->second;
    }
    
    return specialization;
}

SGCore::Ref<SGCore::FontSpecialization> SGCore::Font::getSpecialization
(const SGCore::FontSpecializationSettings& fontSpecializationSettings)
{
    auto foundSpecIter = m_specializations.find(fontSpecializationSettings);
    if(foundSpecIter == m_specializations.end())
    {
        return nullptr;
    }
    else
    {
        return foundSpecIter->second;
    }
}
