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

void SGCore::UI::ANTLRCSSListener::enterDimension(css3Parser::DimensionContext* ctx)
{
    if(ctx->getText().ends_with("px"))
    {
        std::cout << "number in pixels: " << ctx->getText() << std::endl;
    }
}

void SGCore::UI::ANTLRCSSListener::enterCalc(css3Parser::CalcContext* ctx)
{
    processCalculation(ctx);
}

void SGCore::UI::ANTLRCSSListener::processCalculation(antlr4::tree::ParseTree* currentANTLRNode) noexcept
{
    // по моему это базар
    /*
     struct CSSMathNode
     {
        std::vector<Ref<CSSMathNode>> m_operands
        CSSMathSign m_sign = CSSMathSign::NO_SIGN;

        virtual float calculate()
        {
            float result = 0.0f;

            CSSMathSign lastSign = CSSMathSign::ADD;

            for(const auto& operand : m_operands)
            {
                const float calculatedValue = operand->calculate();

                result {lastSign}= calculatedValue;

                lastSign = operand->m_sign;
            }

            return result;
        }
     }

     struct CSSMathNumericNode : CSSMathNode
     {
        float m_value

        float calculate() final
        {
            return m_value;
        }
     }

     calc(30 / (27 + 3) * 10);

     CSSMathNode(
        CSSMathNumericNode(
            float<30>, /
        ),
        CSSMathNode(
            CSSMathNumericNode(
                float<27>, +
            ),
            CSSMathNumericNode(
                float<3>
            )
        ),
        CSSMathNumericNode(
            float<10> *
        )
     )

     calc((30 - (21 - 9)) / (27 + 3) * 10);

     result: 10

     CSSMathNode(
        CSSMathNode(
            CSSMathNumericNode(
                float<30> -
            ),
            CSSMathNode(
                CSSMathNumericNode(
                    float<21> -
                ),
                CSSMathNumericNode(
                    float<9>
                ),
            )
        ), /
        CSSMathNode(
            CSSMathNumericNode(
                float<27> +
            ),
            CSSMathNumericNode(
                float<3>
            ),
        ), *
        CSSMathNumericNode(
            float<10>
        ),
     )

     result: 6
     */

    for(auto* child : currentANTLRNode->children)
    {
        {
            auto* asNestedExpr = dynamic_cast<css3Parser::CalcNestedValueContext*>(child);

            if(asNestedExpr)
            {
                std::cout << "nested expr: " << asNestedExpr->getText() << std::endl;
            }
        }

        {
            auto* asExpr = dynamic_cast<css3Parser::CalcExprContext*>(child);

            if(asExpr)
            {
                std::cout << "expr: " << asExpr->getText() << std::endl;
            }

            /*if(asExpr)
            {
                auto* lOperand = asExpr->calcOperand(0);
                auto* rOperand = asExpr->calcOperand(1);

                {
                    auto* lOperandAsNumber = dynamic_cast<css3Parser::NumberContext*>(lOperand);

                    if(lOperandAsNumber)
                    {
                        std::cout << "l_oper" << lOperandAsNumber->getText() << std::endl;
                    }
                    else
                    {
                        auto* lOperandAsDimension = dynamic_cast<css3Parser::DimensionContext*>(lOperand);

                        if(lOperandAsDimension)
                        {
                            std::cout << "l_oper" << lOperandAsDimension->getText();
                        }
                    }
                }

                if(asExpr->Plus(0))
                {
                    std::cout << " + ";
                }
                else if(asExpr->Minus(0))
                {
                    std::cout << " - ";
                }
                else if(asExpr->Multiply(0))
                {
                    std::cout << " * ";
                }
                else if(asExpr->Divide(0))
                {
                    std::cout << " / ";
                }

                if(rOperand)
                {
                    auto* rOperandAsNumber = dynamic_cast<css3Parser::NumberContext*>(rOperand);

                    if(rOperandAsNumber)
                    {
                        std::cout << "r_oper" << rOperandAsNumber->getText() << std::endl;
                    }
                    else
                    {
                        auto* rOperandAsDimension = dynamic_cast<css3Parser::DimensionContext*>(rOperand);

                        if(rOperandAsDimension)
                        {
                            std::cout << "r_oper" << rOperandAsDimension->getText() << std::endl;
                        }
                    }
                }
            }*/
        }

        processCalculation(child);
    }
}

