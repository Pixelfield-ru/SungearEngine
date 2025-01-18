//
// Created by stuka on 18.01.2025.
//
#include "ANTLRCSSListener.h"

#include "PropertyEnumTypes/CSSFlexDirectionType.h"
#include "CSSPropertyType.h"
#include "SGCore/Memory/AssetManager.h"

void SGCore::UI::ANTLRCSSListener::enterSelector(css3Parser::SelectorContext* ctx)
{
    const std::string currentSelectorName = ctx->children[0]->children[0]->children[1]->getText();

    auto foundSelector = m_toCSSFile->findSelector(currentSelectorName);

    if(foundSelector)
    {
        m_currentSelector = foundSelector.get();

        return;
    }

    auto assetManager = m_toCSSFile->getParentAssetManager();
    const InterpolatedPath newSelectorPath = m_toCSSFile->getPath() / "selectors" / currentSelectorName;

    m_toCSSFile->m_selectors.push_back(assetManager->getOrAddAssetByPath<CSSSelector>(newSelectorPath));

    m_currentSelector = m_toCSSFile->m_selectors.rbegin()->get();

    m_currentSelector->m_name = currentSelectorName;
}

void SGCore::UI::ANTLRCSSListener::enterKnownDeclaration(css3Parser::KnownDeclarationContext* ctx)
{
    if(!m_currentSelector)
    {
        LOG_E(SGCORE_TAG, "ANTLRCSSListener ERROR: can not enter the known declaration without entering "
                          "the selector (declaration of property was found but selector was not found).");

        return;
    }

    const std::string propertyName = ctx->property_()->getText();
    const std::string propertyStringValue = ctx->expr()->getText();

    CSSPropertyType propertyType = getPropertyTypeFromName(propertyName);

    switch(propertyType)
    {
        case CSSPropertyType::PT_FLEX_DIRECTION:
        {
            m_currentSelector->m_flexDirection = getFlexDirectionTypeFromName(propertyStringValue);

            break;
        }
        case CSSPropertyType::PT_FLEX_WRAP:break;
        case CSSPropertyType::PT_JUSTIFY_CONTENT:break;
        case CSSPropertyType::PT_ALIGN_ITEMS:break;
        case CSSPropertyType::PT_ALIGN_CONTENT:break;
        case CSSPropertyType::PT_GAP:break;
        case CSSPropertyType::PT_ORDER:break;
        case CSSPropertyType::PT_FLEX_GROW:break;
        case CSSPropertyType::PT_FLEX_SHRINK:break;
        case CSSPropertyType::PT_WIDTH:
        {

            break;
        }
        case CSSPropertyType::PT_HEIGHT:break;
        case CSSPropertyType::PT_BACKGROUND_COLOR:break;
        case CSSPropertyType::PT_UNKNOWN:break;
    }
}

