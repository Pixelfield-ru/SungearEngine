//
// Created by ilya on 23.02.24.
//

#include <spdlog/spdlog.h>

#include "Font.h"
#include "SGCore/UI/FontsManager.h"
#include "SGCore/Graphics/API/ITexture2D.h"

SGCore::UI::Font::~Font() noexcept
{
    for(auto& [settings, font] : m_specializations)
    {
        font->m_usedFont = nullptr;
    }

    msdfgen::destroyFont(m_fontHandler);
}

msdfgen::FontHandle* SGCore::UI::Font::getFontHandler() const noexcept
{
    return m_fontHandler;
}

void SGCore::UI::Font::doLoad(const InterpolatedPath& path)
{
    const auto ftLib = FontsManager::getInstance().getFTLibrary();

    m_fontHandler = msdfgen::loadFont(ftLib, Utils::toUTF8(path.resolved().u16string()).c_str());
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

        specialization->m_usedFont = this;
        
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
