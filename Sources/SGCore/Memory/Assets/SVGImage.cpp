//
// Created by ilya on 21.05.24.
//

#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>
#include "SVGImage.h"

void SGCore::SVGImage::doLoad(const std::filesystem::path& path)
{
    m_document = lunasvg::Document::loadFromFile(Utils::toUTF8(path.u16string()));
}

SGCore::Ref<SGCore::SVGImageSpecialization> SGCore::SVGImage::getSpecialization(const std::uint32_t& width, const std::uint32_t& height) noexcept
{
    if(!m_document)
    {
        LOG_E(SGCORE_TAG,
              "Cannot create specialization (with width: '{}', height: '{}') of SVGImage loaded at path '{}': document equals to nullptr (SVGImage was not successfully loaded previously).",
              m_path.string(),
              width,
              height);
        
        return nullptr;
    }
    
    Ref<SVGImageSpecialization> specialization;
    
    auto foundIt = std::find_if(m_specializations.begin(), m_specializations.end(), [&width, &height](const auto& spec) {
        return width == spec->getSize().x && height == spec->getSize().y;
    });
    
    if(foundIt == m_specializations.end())
    {
        auto bitmap = m_document->renderToBitmap(width, height);
        bitmap.convertToRGBA();
        
        if(!bitmap.valid())
        {
            LOG_E(SGCORE_TAG,
                  "Cannot create specialization (with width: '{}', height: '{}') of SVGImage loaded at path '{}': bitmap was not loaded successfully (not valid).",
                  m_path.string(),
                  width,
                  height);
            
            return nullptr;
        }
        
        specialization = MakeRef<SVGImageSpecialization>();
        specialization->m_bitmap = std::move(bitmap);
        specialization->regenerate();
        
        m_specializations.push_back(specialization);
    }
    else
    {
        specialization = *foundIt;
    }
    
    return specialization;
}

void SGCore::SVGImage::removeSpecialization(const uint32_t& width, const uint32_t& height) noexcept
{
    std::erase_if(m_specializations, [&width, &height](const auto& specialization) {
        return width == specialization->getSize().x && height == specialization->getSize().y;
    });
}
