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
        LOG_E(SGCORE_TAG,
              "ANTLRCSSListener ERROR: can not enter the known declaration without entering "
              "the selector (declaration of property was found but selector was not found). In CSS file: '{}'",
              Utils::toUTF8(m_toCSSFile->getPath().resolved().u16string()));

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
            if(ctx->expr()->term().size() > 1) // invalid count of terms. ignoring property...
            {
                printInvalidCountOfTermsInPropertyError(propertyName, ctx->expr()->term().size(), 1);

                break;
            }

            auto* knownTerm = dynamic_cast<css3Parser::KnownTermContext*>(ctx->expr()->term(0));

            if(knownTerm)
            {
                if(knownTerm->calc()) // width: calc(...)
                {
                    auto mathNode = MakeRef<CSSMathNode>();

                    processCalculation(knownTerm->calc(), mathNode);
                    mathNode->resolvePriorities();

                    std::cout << "width calc: " << mathNode->calculate() << std::endl;
                }
            }
            else // we have bad term. ignoring property...
            {
                printBadTermInPropertyError(propertyName, 0, ctx->expr()->term(0)->getText());

                break;
            }

            break;
        }
        case CSSPropertyType::PT_HEIGHT:break;
        case CSSPropertyType::PT_BACKGROUND_COLOR:break;
        case CSSPropertyType::PT_UNKNOWN:break;
    }
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

                    processCalculation(asExpr->calcOperand(i)->calcValue()->calcNestedValue(), newParentMathNode);

                    continue;
                }
                else if(asExpr->calcOperand(i)->calcValue()->number())
                {
                    auto newNumberNode = MakeRef<CSSMathNumericNode>();

                    newNumberNode->m_value = std::stof(asExpr->calcOperand(i)->calcValue()->number()->getText());

                    currentParentMathNode->m_operands.push_back(newNumberNode);

                    continue;
                }
                else if(asExpr->calcOperand(i)->calcValue()->dimension())
                {
                    auto newNumberNode = MakeRef<CSSMathNumericNode>();

                    const std::string dimension = asExpr->calcOperand(i)->calcValue()->dimension()->Dimension()->getText();

                    #pragma region Units
                    // ================================================================ absolute length
                    if(dimension.ends_with("px"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_PX;
                    }
                    else if(dimension.ends_with("cm"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_CM;
                    }
                    else if(dimension.ends_with("mm"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_MM;
                    }
                    else if(dimension.ends_with("in"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_IN;
                    }
                    else if(dimension.ends_with("pt"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_PT;
                    }
                    else if(dimension.ends_with("pc"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_PC;
                    }
                    else if(dimension.ends_with("q"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_Q;
                    }
                    // ================================================================ angle
                    else if(dimension.ends_with("deg"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_DEG;
                    }
                    else if(dimension.ends_with("rad"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_RAD;
                    }
                    else if(dimension.ends_with("grad"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_GRAD;
                    }
                    else if(dimension.ends_with("turn"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_TURN;
                    }
                    // ================================================================ time
                    else if(dimension.ends_with("ms"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_MS;
                    }
                    else if(dimension.ends_with("s"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_S;
                    }
                    // ================================================================ frequency
                    else if(dimension.ends_with("hz"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_HZ;
                    }
                    else if(dimension.ends_with("khz"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_KHZ;
                    }
                    // ================================================================ relative units
                    else if(dimension.ends_with("em"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_EM;
                    }
                    else if(dimension.ends_with("ex"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_EX;
                    }
                    else if(dimension.ends_with("ch"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_CH;
                    }
                    else if(dimension.ends_with("rem"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_REM;
                    }
                    else if(dimension.ends_with("vw"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_VW;
                    }
                    else if(dimension.ends_with("vh"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_VH;
                    }
                    else if(dimension.ends_with("vmin"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_VMIN;
                    }
                    else if(dimension.ends_with("vmax"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_VMAX;
                    }
                    // ================================================================ resolution units
                    else if(dimension.ends_with("dpi"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_DPI;
                    }
                    else if(dimension.ends_with("dpcm"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_DPCM;
                    }
                    else if(dimension.ends_with("dppx"))
                    {
                        newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_DPPX;
                    }
                    #pragma endregion Units

                    // stof ignores chars after number so we dont care that we have 123px for example
                    newNumberNode->m_value = std::stof(dimension);

                    if(asExpr->calcOperand(i)->calcValue()->dimension()->Minus())
                    {
                        newNumberNode->m_value *= -1.0f;
                    }

                    currentParentMathNode->m_operands.push_back(newNumberNode);

                    continue;
                }
                else if(asExpr->calcOperand(i)->calcValue()->unknownDimension())
                {
                    auto newNumberNode = MakeRef<CSSMathNumericNode>();

                    newNumberNode->m_value = std::stof(asExpr->calcOperand(i)->calcValue()->unknownDimension()->UnknownDimension()->getText());
                    newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_PX;

                    LOG_W(SGCORE_TAG,
                          "ANTLRCSSListener calculation processing warning: calculation "
                          "expression has unknown dimension: '{}'. Note: The default qualifier ('px') "
                          "is assigned to the operand.\nIn CSS file: '{}'",
                          asExpr->calcOperand(i)->calcValue()->unknownDimension()->getText(),
                          Utils::toUTF8(m_toCSSFile->getPath().resolved().u16string()));

                    if(asExpr->calcOperand(i)->calcValue()->unknownDimension()->Minus())
                    {
                        newNumberNode->m_value *= -1.0f;
                    }

                    currentParentMathNode->m_operands.push_back(newNumberNode);

                    continue;
                }
                else if(asExpr->calcOperand(i)->calcValue()->percentage())
                {
                    auto newNumberNode = MakeRef<CSSMathNumericNode>();

                    newNumberNode->m_value = std::stof(asExpr->calcOperand(i)->calcValue()->percentage()->Percentage()->getText());
                    newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_PERCENT;

                    if(asExpr->calcOperand(i)->calcValue()->percentage()->Minus())
                    {
                        newNumberNode->m_value *= -1.0f;
                    }

                    currentParentMathNode->m_operands.push_back(newNumberNode);

                    continue;
                }
                else if(asExpr->calcOperand(i)->calcValue()->calc())
                {
                    auto newParentMathNode = MakeRef<CSSMathNode>();

                    currentParentMathNode->m_operands.push_back(newParentMathNode);

                    processCalculation(asExpr->calcOperand(i)->calcValue()->calc(), newParentMathNode);

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

void SGCore::UI::ANTLRCSSListener::printInvalidCountOfTermsInPropertyError(const std::string& propertyName,
                                                                           const size_t& currentTermsCount,
                                                                           const std::int64_t& validTermsMinCount,
                                                                           const std::int64_t& validTermsMaxCount) const noexcept
{
    std::int64_t finalValidTermsMaxCount = validTermsMaxCount;

    if(finalValidTermsMaxCount < 0)
    {
        finalValidTermsMaxCount = validTermsMinCount;
    }

    LOG_E(SGCORE_TAG,
          "ANTLRCSSListener can not process property '{}': property has invalid count of terms in section 'value'. "
          "Property was ignored. "
          "Count of terms: '{}'. "
          "Valid count of terms: (min: '{}', max: '{}').\n"
          "In CSS file: {}",
          propertyName,
          currentTermsCount,
          validTermsMinCount,
          finalValidTermsMaxCount,
          Utils::toUTF8(m_toCSSFile->getPath().resolved().u16string()));
}

void
SGCore::UI::ANTLRCSSListener::printBadTermInPropertyError(const std::string& propertyName,
                                                          const int64_t& termIndex,
                                                          const std::string& termValue) const noexcept
{
    LOG_E(SGCORE_TAG,
          "ANTLRCSSListener can not process property '{}': property has invalid term in section 'value'. "
          "Property was ignored. Term index: '{}'. Term value: '{}'.\n"
          "In CSS file: {}",
          propertyName,
          termIndex,
          termValue,
          Utils::toUTF8(m_toCSSFile->getPath().resolved().u16string()));
}

