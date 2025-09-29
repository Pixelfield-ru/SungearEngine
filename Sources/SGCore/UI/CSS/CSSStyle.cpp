//
// Created by stuka on 18.01.2025.
//
#include "CSSStyle.h"

#include "SGCore/UI/FontsManager.h"

SGCore::UI::CSSStyle::CSSStyle() noexcept
{
    // setting default font
    m_font = FontsManager::getInstance().getAssetManager()->getAsset<Font, AssetStorageType::BY_ALIAS>("JetBrains Mono");

    // using 64 to generate big MSDF atlas
    m_fontSpecializationSettings.m_height = 64;
    setFontSpecializationSettings(m_fontSpecializationSettings);
}

const std::string& SGCore::UI::CSSStyle::getSelector() const noexcept
{
    return m_selector;
}

const std::string& SGCore::UI::CSSStyle::getPseudoName() const noexcept
{
    return m_pseudoClass;
}

void SGCore::UI::CSSStyle::calculateCache(const UIElementCache* parentElementCache,
                                             UIElementCache& thisElementCache) noexcept
{
    if(m_padding.containsAlternative())
    {
        auto topPadding = m_padding.getFromAlternativeValue<0>();
        auto rightPadding = m_padding.getFromAlternativeValue<1>();
        auto bottomPadding = m_padding.getFromAlternativeValue<2>();
        auto leftPadding = m_padding.getFromAlternativeValue<3>();

        if(topPadding)
        {
            thisElementCache.m_topPadding = topPadding->calculate(&thisElementCache.m_finalSize.y);
        }

        if(rightPadding)
        {
            thisElementCache.m_rightPadding = rightPadding->calculate(&thisElementCache.m_finalSize.x);
        }

        if(bottomPadding)
        {
            thisElementCache.m_bottomPadding = bottomPadding->calculate(&thisElementCache.m_finalSize.y);
        }

        if(leftPadding)
        {
            thisElementCache.m_leftPadding = leftPadding->calculate(&thisElementCache.m_finalSize.x);
        }
    }

    if(m_gap.containsAlternative())
    {
        thisElementCache.m_gap.x = m_gap.getFromAlternativeValue<0>()->calculate(&thisElementCache.m_finalSize.x);
        thisElementCache.m_gap.y = m_gap.getFromAlternativeValue<1>()->calculate(&thisElementCache.m_finalSize.y);
    }

    if(m_width.containsAlternative())
    {
        thisElementCache.m_basicSize.x = m_width.getFromAlternativeValue<0>()->calculate(
            parentElementCache ? &parentElementCache->m_finalSize.x : nullptr);
    }

    if(m_height.containsAlternative())
    {
        thisElementCache.m_basicSize.y = m_height.getFromAlternativeValue<0>()->calculate(
            parentElementCache ? &parentElementCache->m_finalSize.y : nullptr);
    }
    else
    {
        const PositionAndSizeKeyword keyword = m_height.getKeyword();

        if(keyword == PositionAndSizeKeyword::KW_AUTO)
        {
            thisElementCache.m_basicSize.y = 0.0f;
        }
    }

    thisElementCache.m_finalSize.x = thisElementCache.m_basicSize.x;
    thisElementCache.m_finalSize.y = thisElementCache.m_basicSize.y;

    if(m_backgroundColor.containsAlternative())
    {
        thisElementCache.m_backgroundColor.r = m_backgroundColor.getFromAlternativeValue<0>()->calculate(
            parentElementCache ? &parentElementCache->m_backgroundColor.r : nullptr);

        thisElementCache.m_backgroundColor.g = m_backgroundColor.getFromAlternativeValue<1>()->calculate(
            parentElementCache ? &parentElementCache->m_backgroundColor.g : nullptr);

        thisElementCache.m_backgroundColor.b = m_backgroundColor.getFromAlternativeValue<2>()->calculate(
            parentElementCache ? &parentElementCache->m_backgroundColor.b : nullptr);

        thisElementCache.m_backgroundColor.a = m_backgroundColor.getFromAlternativeValue<3>()->calculate(
            parentElementCache ? &parentElementCache->m_backgroundColor.a : nullptr);
    }

    if(m_topLeftBorderRadius.containsAlternative())
    {
        const auto& alternativeValue = m_topLeftBorderRadius.getFromAlternativeValue<0>();

        thisElementCache.m_borderRadiusCache[0].x = alternativeValue.m_radiusX->calculate(&thisElementCache.m_finalSize.x);

        thisElementCache.m_borderRadiusCache[0].y = alternativeValue.m_radiusY->calculate(&thisElementCache.m_finalSize.y);
    }

    if(m_topRightBorderRadius.containsAlternative())
    {
        const auto& alternativeValue = m_topRightBorderRadius.getFromAlternativeValue<0>();

        thisElementCache.m_borderRadiusCache[1].x = alternativeValue.m_radiusX->calculate(&thisElementCache.m_finalSize.x);

        thisElementCache.m_borderRadiusCache[1].y = alternativeValue.m_radiusY->calculate(&thisElementCache.m_finalSize.y);
    }

    if(m_bottomRightBorderRadius.containsAlternative())
    {
        const auto& alternativeValue = m_bottomRightBorderRadius.getFromAlternativeValue<0>();

        thisElementCache.m_borderRadiusCache[2].x = alternativeValue.m_radiusX->calculate(&thisElementCache.m_finalSize.x);

        thisElementCache.m_borderRadiusCache[2].y = alternativeValue.m_radiusY->calculate(&thisElementCache.m_finalSize.y);
    }

    if(m_bottomLeftBorderRadius.containsAlternative())
    {
        const auto& alternativeValue = m_bottomLeftBorderRadius.getFromAlternativeValue<0>();

        thisElementCache.m_borderRadiusCache[3].x = alternativeValue.m_radiusX->calculate(&thisElementCache.m_finalSize.x);

        thisElementCache.m_borderRadiusCache[3].y = alternativeValue.m_radiusY->calculate(&thisElementCache.m_finalSize.y);
    }

    if(m_fontSize.containsAlternative())
    {
        thisElementCache.m_fontSize = m_fontSize.getFromAlternativeValue<0>()->calculate(parentElementCache ? &parentElementCache->m_fontSize : nullptr);
    }
    else if(m_fontSize.containsKeyword())
    {
        // todo: make for keyword
    }
}

void SGCore::UI::CSSStyle::setFontSpecializationSettings(const FontSpecializationSettings& settings) noexcept
{
    m_fontSpecializationSettings = settings;

    auto lockedFont = m_font.lock();

    if(lockedFont)
    {
        if(!lockedFont->isSpecializationExists(m_fontSpecializationSettings))
        {
            auto specialization = lockedFont->addOrGetSpecialization(m_fontSpecializationSettings);

            specialization->parse('A', 'Z');
            specialization->parse(u'a', u'z');
            specialization->parse(u'А', u'Я');
            specialization->parse(u'а', u'я');
            specialization->parse({ u'ё', u'Ё' });
            specialization->parse('0', '9');
            specialization->parse({ u'!', u'@', u'"', u'\'', u'№', u'#', u'$', u';', u'%', u'^', u':', u'&', u'*', u'(', u')', u'-', u'–', u'—', u'_', u'+', u'=', u'|', u'\\', u'/', u'.', u',', u'`', u'{', u'}', u'[', u']', u'~', u' ', u'<', u'>' });
            specialization->parse({ U'😊', U'😁', U'😃', U'😄', U'😅', U'😂', U'😍', U'😜', U'😎', U'🤔',
                                                U'🤗', U'😔', U'😢', U'😡', U'🤯', U'🤩', U'🙄', U'🤣', U'🥺', U'😏',
                                                U'😳', U'😜', U'😤', U'🤪', U'😈', U'👀', U'👋', U'🙌', U'🤝', U'💪',
                                                U'✌', U'🙏', U'👐', U'💔',  U'\u2764', U'💥', U'🌟', U'🎉', U'🎶', U'🎧',
                                                U'🕺', U'💃', U'🚀'  });

            specialization->createAtlas();
            specialization->saveAtlasAsTexture(m_fontSpecializationSettings.m_name + ".png");
            // specialization->saveAtlasSDFAsTexture(m_fontSpecializationSettings.m_name + "_sdf.png");

            m_fontSpecialization = specialization;
        }
        else
        {
            m_fontSpecialization = lockedFont->getSpecialization(m_fontSpecializationSettings);
        }
    }
}

const SGCore::UI::FontSpecializationSettings& SGCore::UI::CSSStyle::getFontSpecializationSettings() const noexcept
{
    return m_fontSpecializationSettings;
}

void SGCore::UI::CSSStyle::updateFontSettings() noexcept
{
    setFontSpecializationSettings(m_fontSpecializationSettings);
}

SGCore::Ref<SGCore::UI::FontSpecialization> SGCore::UI::CSSStyle::getFontSpecialization() const noexcept
{
    return m_fontSpecialization.lock();
}

void SGCore::UI::CSSStyle::copy(CSSStyle& to) const noexcept
{
    to.m_display = m_display;
    to.m_flexDirection = m_flexDirection;
    to.m_flexWrap = m_flexWrap;

    to.m_width = m_width;
    to.m_height = m_height;

    to.m_padding = m_padding;

    to.m_gap = m_gap;

    to.m_bottomLeftBorderRadius = m_bottomLeftBorderRadius;
    to.m_topLeftBorderRadius = m_topLeftBorderRadius;
    to.m_topRightBorderRadius = m_topRightBorderRadius;
    to.m_bottomRightBorderRadius = m_bottomRightBorderRadius;

    to.m_backgroundColor = m_backgroundColor;

    to.m_font = m_font;

    to.m_fontSize = m_fontSize;

    to.m_fontSpecializationSettings = m_fontSpecializationSettings;
    to.m_fontSpecialization = m_fontSpecialization;

    to.m_selector = m_selector;
}

void SGCore::UI::CSSStyle::doLoad(const InterpolatedPath& path)
{
    LOG_NOT_SUPPORTED_FUNC(SGCORE_TAG);
}

void SGCore::UI::CSSStyle::doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept
{

}

void SGCore::UI::CSSStyle::doReloadFromDisk(AssetsLoadPolicy loadPolicy,
                                               Ref<Threading::Thread> lazyLoadInThread) noexcept
{
    LOG_NOT_SUPPORTED_FUNC(SGCORE_TAG);
}
