//
// Created by stuka on 18.01.2025.
//
#include "ANTLRCSSListener.h"
#include "PropertyEnumTypes/CSSFlexDirectionType.h"
#include "CSSPropertyType.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/UI/CSS/Math/CSSMathNode.h"
#include "SGCore/UI/CSS/Math/CSSMathNumericNode.h"

void SGCore::UI::ANTLRCSSListener::enterSelector(css3Parser::SelectorContext* ctx)
{
    std::string currentSelectorName = "";
    if(!ctx->children.empty())
    {
        if(!ctx->children[0]->children.empty())
        {
            if(ctx->children[0]->children[0]->children.size() > 1)
            {
                currentSelectorName = ctx->children[0]->children[0]->children[1]->getText();
            }
        }
    }

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
            auto mathNode = MakeRef<CSSMathNode>();

            break;
        }
        case CSSPropertyType::PT_HEIGHT:break;
        case CSSPropertyType::PT_BACKGROUND_COLOR:break;
        case CSSPropertyType::PT_UNKNOWN:break;
    }
}

void SGCore::UI::ANTLRCSSListener::enterDimension(css3Parser::DimensionContext* ctx)
{
    if(ctx->getText().ends_with("px"))
    {
        std::cout << "number in pixels: " << ctx->getText() << std::endl;
    }
}

void SGCore::UI::ANTLRCSSListener::enterCalc(css3Parser::CalcContext* ctx)
{
    auto mathNode = MakeRef<CSSMathNode>();
    processCalculation(ctx, mathNode);
    mathNode->resolvePriorities();

    std::cout << "calculation result: " << mathNode->calculate() << std::endl;
}

void SGCore::UI::ANTLRCSSListener::processCalculation(antlr4::tree::ParseTree* currentANTLRNode,
                                                      const Ref<CSSMathNode>& currentParentMathNode) noexcept
{
    for(auto* child : currentANTLRNode->children)
    {
        auto* asExpr = dynamic_cast<css3Parser::CalcExprContext*>(child);

        if(asExpr)
        {
            const std::int64_t operandsCount = std::ssize(asExpr->calcOperand());

            for(std::int64_t i = 0; i < operandsCount; ++i)
            {
                const std::int64_t signI = i - 1;

                if(signI >= 0 && signI < asExpr->calcSign().size())
                {
                    auto* currentSign = asExpr->calcSign(signI);

                    if(currentSign)
                    {
                        auto currentOperandSign = CSSMathSign::MS_NO_SIGN;

                        if(currentSign->Plus())
                        {
                            currentOperandSign = CSSMathSign::MS_PLUS;
                        }
                        else if(currentSign->Minus())
                        {
                            currentOperandSign = CSSMathSign::MS_MINUS;
                        }
                        else if(currentSign->Multiply())
                        {
                            currentOperandSign = CSSMathSign::MS_MULTIPLY;
                        }
                        else if(currentSign->Divide())
                        {
                            currentOperandSign = CSSMathSign::MS_DIVIDE;
                        }

                        currentParentMathNode->m_operands[signI]->m_sign = currentOperandSign;
                    }
                }

                if(asExpr->calcOperand(i)->calcValue()->calcNestedValue())
                {
                    auto newParentMathNode = MakeRef<CSSMathNode>();

                    if(asExpr->calcOperand(i)->calcValue()->calcNestedValue()->Minus())
                    {
                        newParentMathNode->m_unarySign = CSSMathSign::MS_MINUS;
                    }

                    currentParentMathNode->m_operands.push_back(newParentMathNode);

                    processCalculation(asExpr->calcOperand(i), newParentMathNode);

                    continue;
                }
                else if(asExpr->calcOperand(i)->calcValue()->number())
                {
                    auto newNumberNode = MakeRef<CSSMathNumericNode>();

                    newNumberNode->m_value = std::stof(asExpr->calcOperand(i)->calcValue()->number()->getText());

                    currentParentMathNode->m_operands.push_back(newNumberNode);

                    continue;
                }
            }
        }
        else
        {
            processCalculation(child, currentParentMathNode);
        }
    }
}

