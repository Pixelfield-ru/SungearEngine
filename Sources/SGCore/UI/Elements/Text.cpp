//
// Created by stuka on 16.03.2025.
//

#include "Text.h"

#include "SGCore/UI/FontSpecializationRenderer.h"

SGCore::UI::Text::Text() noexcept
{
    m_selector = AssetManager::getInstance()->getOrAddAssetByAlias<CSSSelector>("sgui_default_text_style");
}

bool SGCore::UI::Text::draw(const LayeredFrameReceiver::reg_t& cameraReceiver,
                            const Transform& elementTransform,
                            UIElementCache& elementCache) noexcept
{
    if(!m_selector) return true;

    AssetRef<Font> font = m_selector->m_font.lock();

    if(font)
    {
        Ref<FontSpecialization> fontSpec = font->getSpecialization(m_selector->getFontSpecializationSettings());

        if(fontSpec)
        {
            fontSpec->getRenderer()->drawText(this, elementTransform, elementCache);
        }
    }

    return true;
}

void SGCore::UI::Text::doCalculateLayout(const UIElementCache* parentElementCache, UIElementCache& thisElementCache,
                                         const Transform* parentTransform, Transform& ownTransform) noexcept
{
    if(m_selector)
    {
        m_selector->calculateCache(parentElementCache, thisElementCache);

        thisElementCache.m_finalSize.y = m_textSize.y;
    }
    else
    {

    }
}

void SGCore::UI::Text::doGenerateMeshBaseSelector(const UIElementCache* parentElementCache,
                                                  UIElementCache& thisElementCache) noexcept
{

}

void SGCore::UI::Text::doGenerateBasicMesh() noexcept
{

}
