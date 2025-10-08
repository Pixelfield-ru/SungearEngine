//
// Created by stuka on 24.09.2025.
//

#include "UIIncludeNodeProcessor.h"

#include "SGCore/UI/UIElementAttributeType.h"
#include "SGCore/UI/CSS/CSSFile.h"
#include "UINodesProcessorsUtils.h"

Ref<SGCore::UI::UIElement> SGCore::UI::UIIncludeNodeProcessor::allocateElement() noexcept
{
    return nullptr;
}

void SGCore::UI::UIIncludeNodeProcessor::processElement(UIDocument* inDocument,
                                                        const Ref<UIElement>& element,
                                                        const pugi::xml_node& elementNode) noexcept
{
    std::string includeType;
    std::string includePath;

    for(const auto& attribute : elementNode.attributes())
    {
        const UIElementAttributeType attributeType = getUIElementAttributeTypeFromString(attribute.name());

        if(attributeType == UIElementAttributeType::AT_UNKNOWN)
        {
            UINodesProcessorsUtils::printUnknownAttributeError(inDocument, attribute, elementNode);

            continue;
        }

        switch(attributeType)
        {
            case UIElementAttributeType::AT_TYPE:
            {
                includeType = attribute.value();

                break;
            }
            case UIElementAttributeType::AT_PATH:
            {
                includePath = attribute.value();

                break;
            }
            default:
            {
                break;
            }
        }
    }

    const InterpolatedPath resolvedIncludePath = UINodesProcessorsUtils::resolveIncludePath(inDocument, includePath);

    if(includeType == "stylesheet")
    {
        auto cssFile = inDocument->getParentAssetManager()->loadAsset<CSSFile>(resolvedIncludePath);

        if(cssFile)
        {
            inDocument->m_includedCSSFiles.push_back(cssFile);
        }
    }
    else if(includeType == "xml")
    {
        auto uiDocument = inDocument->getParentAssetManager()->loadAsset<UIDocument>(resolvedIncludePath);

        if(!uiDocument)
        {
            LOG_E(SGCORE_TAG, "In UI document '{}': can not include XML file by path '{}': this path does not exist.",
                  Utils::toUTF8(inDocument->getPath().resolved().u16string()),
                  Utils::toUTF8(resolvedIncludePath.resolved().u16string()))
            return;
        }

        inDocument->m_includedUIDocuments.push_back(uiDocument);
    }
}
