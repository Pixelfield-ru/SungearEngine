//
// Created by ilya on 23.02.24.
//

#include <spdlog/spdlog.h>

#include "Font.h"
#include "SGCore/UI/FontsManager.h"
#include "SGCore/Graphics/API/ITexture2D.h"

void SGCore::UI::Font::doLoad(const InterpolatedPath& path)
{

}

SGCore::Ref<SGCore::UI::FontSpecialization>
SGCore::UI::Font::addOrGetSpecialization(const FontSpecializationSettings& fontSpecializationSettings)
{
    Ref<FontSpecialization> specialization;
    auto foundSpecIter = m_specializations.find(fontSpecializationSettings);
    if(foundSpecIter == m_specializations.end())
    {
        specialization = MakeRef<FontSpecialization>();
        specialization->m_settings = fontSpecializationSettings;
        
        specialization->prepareToBuild(getPath().resolved().string());
        
        m_specializations[fontSpecializationSettings] = specialization;
    }
    else
    {
        specialization = foundSpecIter->second;
    }
    
    return specialization;
}

SGCore::Ref<SGCore::UI::FontSpecialization> SGCore::UI::Font::getSpecialization
(const SGCore::UI::FontSpecializationSettings& fontSpecializationSettings)
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

bool SGCore::UI::Font::isSpecializationExists(const FontSpecializationSettings& fontSpecializationSettings) const noexcept
{
    return m_specializations.contains(fontSpecializationSettings);
}

void SGCore::UI::Font::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{

}

void SGCore::UI::Font::doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept
{

}
