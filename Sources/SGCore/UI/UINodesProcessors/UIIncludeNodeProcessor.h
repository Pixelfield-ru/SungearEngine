//
// Created by stuka on 25.01.2025.
//

#ifndef SUNGEARENGINE_UIINCLUDENODEPROCESSOR_H
#define SUNGEARENGINE_UIINCLUDENODEPROCESSOR_H

#include "UIElementNodeProcessor.h"
#include "SGCore/UI/UIElementAttributeType.h"
#include "UINodesProcessorsUtils.h"
#include "CSS/CSSFile.h"

namespace SGCore::UI
{
    template<>
    struct UIElementNodeProcessor<UIElementType::ET_INCLUDE>
    {
        static constexpr UIElementType element_type = UIElementType::ET_INCLUDE;

        static void processElement(UIDocument* inDocument,
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
        }
    };
}

#endif //SUNGEARENGINE_UIINCLUDENODEPROCESSOR_H
