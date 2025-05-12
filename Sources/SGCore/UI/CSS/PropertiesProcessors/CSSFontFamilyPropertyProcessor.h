//
// Created by stuka on 11.05.2025.
//

#ifndef SUNGEARENGINE_CSSFONTFAMILYPROPERTYPROCESSOR_H
#define SUNGEARENGINE_CSSFONTFAMILYPROPERTYPROCESSOR_H

#include "SGCore/UI/FontsManager.h"
#include "SGCore/UI/ANTLR4CSS3Generated/css3Parser.h"
#include "SGCore/UI/CSS/CSSPropertyType.h"
#include "SGCore/UI/CSS/PropertyValues/CSSPropertyValueKeywords.h"
#include "SGCore/UI/CSS/CSSSelector.h"

namespace SGCore::UI
{
    template<>
    struct CSSPropertyProcessor<CSSPropertyType::PT_FONT_FAMILY>
    {
        static constexpr FontFamilyKeyword property_default_value = FontFamilyKeyword::KW_UNKNOWN;
        static inline const std::string property_default_value_str = "unknown";

        static void processProperty(ANTLRCSSListener* antlrcssListener,
                                    css3Parser::KnownDeclarationContext* propertyDeclContext,
                                    const std::string& propertyName,
                                    const std::string& propertyCurrentValue) noexcept
        {
            CSSSelector* currentSelector = antlrcssListener->m_currentSelector;

            auto* expr = propertyDeclContext->expr();

            AssetRef<Font> foundFont = nullptr;

            for(size_t i = 0; i < expr->term().size(); ++i)
            {
                std::string fontFamily = expr->term(i)->getText();
                if(fontFamily.size() >= 2)
                {
                    // erasing first " and last "
                    fontFamily.erase(fontFamily.begin());
                    fontFamily.erase(fontFamily.end() - 1);
                }

                std::cout << "font family: " << fontFamily << std::endl;

                foundFont = FontsManager::getInstance().getAssetManager()->getAsset<Font, AssetStorageType::BY_ALIAS>(fontFamily);

                if(foundFont)
                {
                    currentSelector->m_font = foundFont;
                    currentSelector->updateFontSettings();

                    std::cout << "found font with family: " << fontFamily << std::endl;

                    break;
                }
            }
        }
    };
}

#endif // SUNGEARENGINE_CSSFONTFAMILYPROPERTYPROCESSOR_H
