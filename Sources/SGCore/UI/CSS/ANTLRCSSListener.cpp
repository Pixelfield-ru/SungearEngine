//
// Created by stuka on 18.01.2025.
//
#include "ANTLRCSSListener.h"
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

    const std::string propertyDefaultValue = getDefaultPropertyValue(propertyType);

    switch(propertyType)
    {
        case CSSPropertyType::PT_FLEX_DIRECTION:
        {
            m_currentSelector->m_flexDirection = getFlexboxKeywordFromStringValue(propertyStringValue);

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
            m_currentSelector->m_width.m_value = PositionAndSizeKeyword::KW_AUTO;

            if(ctx->expr()->term().size() > 1) // invalid count of terms. ignoring property...
            {
                printInvalidCountOfTermsInPropertyError(propertyName, propertyDefaultValue, ctx->expr()->term().size(), 1);

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

                    m_currentSelector->m_width.m_value = mathNode;
                }
                else if(knownTerm->number())
                {
                    auto mathNode = MakeRef<CSSMathNumericNode>();
                    mathNode->m_value = std::stof(knownTerm->number()->getText());

                    std::cout << "width calc: " << mathNode->calculate() << std::endl;

                    m_currentSelector->m_width.m_value = mathNode;
                }
                else if(knownTerm->dimension())
                {
                    auto mathNode = MakeRef<CSSMathNumericNode>();

                    const std::string dimension = knownTerm->dimension()->Dimension()->getText();

                    mathNode->m_dimensionQualifier = getDimensionFromString(dimension);
                    mathNode->m_value = std::stof(dimension);

                    std::cout << "width calc: " << mathNode->calculate() << std::endl;

                    m_currentSelector->m_width.m_value = mathNode;
                }
                else if(knownTerm->percentage())
                {
                    auto mathNode = MakeRef<CSSMathNumericNode>();

                    const std::string percentage = knownTerm->percentage()->Percentage()->getText();

                    mathNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_PERCENT;
                    mathNode->m_value = std::stof(percentage);

                    std::cout << "width calc: " << mathNode->calculate() << std::endl;

                    m_currentSelector->m_width.m_value = mathNode;
                }
                else // we have keyword
                {
                    const PositionAndSizeKeyword keyword = getPositionAndSizeKeywordFromStringValue(propertyStringValue);

                    if(keyword == PositionAndSizeKeyword::KW_UNKNOWN)
                    {
                        printUnknownKeywordUsedError(propertyName, propertyStringValue, propertyDefaultValue);

                        break;
                    }

                    m_currentSelector->m_width.m_value = keyword;
                }
            }
            else // we have bad term. ignoring property...
            {
                printBadTermInPropertyError(propertyName, 0, ctx->expr()->term(0)->getText(), propertyDefaultValue);

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

                    newNumberNode->m_dimensionQualifier = getDimensionFromString(dimension);

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
                                                                           const std::string& defaultSetKeyword,
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
          "ANTLRCSSListener can not process property '{}' correctly: property has invalid count of terms in section 'value'. "
          "Property has been set to the default value. "
          "Count of terms: '{}'. "
          "Valid count of terms: (min: '{}', max: '{}'). "
          "Set keyword (default): '{}'.\n"
          "In CSS file: {}",
          propertyName,
          currentTermsCount,
          validTermsMinCount,
          finalValidTermsMaxCount,
          defaultSetKeyword,
          Utils::toUTF8(m_toCSSFile->getPath().resolved().u16string()));
}

void
SGCore::UI::ANTLRCSSListener::printBadTermInPropertyError(const std::string& propertyName,
                                                          const int64_t& termIndex,
                                                          const std::string& termValue,
                                                          const std::string& defaultSetKeyword) const noexcept
{
    LOG_E(SGCORE_TAG,
          "ANTLRCSSListener can not process property '{}' correctly: property has invalid term in section 'value'. "
          "Property has been set to the default value. Term index: '{}'. Term value: '{}'. "
          "Set keyword (default): '{}'.\n"
          "In CSS file: {}",
          propertyName,
          termIndex,
          termValue,
          defaultSetKeyword,
          Utils::toUTF8(m_toCSSFile->getPath().resolved().u16string()));
}

void SGCore::UI::ANTLRCSSListener::printUnknownKeywordUsedError(const std::string& propertyName,
                                                                const std::string& currentKeyword,
                                                                const std::string& defaultSetKeyword) const noexcept
{
    LOG_E(SGCORE_TAG,
          "ANTLRCSSListener can not process property '{}' correctly: property has unknown keyword in section 'value'. "
          "Property has been set to the default value. "
          "Current keyword: '{}'. "
          "Set keyword (default): '{}'.\n"
          "In CSS file: {}",
          propertyName,
          currentKeyword,
          defaultSetKeyword,
          Utils::toUTF8(m_toCSSFile->getPath().resolved().u16string()));
}

SGCore::UI::CSSDimensionQualifier
SGCore::UI::ANTLRCSSListener::getDimensionFromString(const std::string& dimensionStr) noexcept
{
    #pragma region Units
    // ================================================================ absolute length
    if(dimensionStr.ends_with("px"))
    {
        return CSSDimensionQualifier::DQ_PX;
    }
    else if(dimensionStr.ends_with("cm"))
    {
        return CSSDimensionQualifier::DQ_CM;
    }
    else if(dimensionStr.ends_with("mm"))
    {
        return CSSDimensionQualifier::DQ_MM;
    }
    else if(dimensionStr.ends_with("in"))
    {
        return CSSDimensionQualifier::DQ_IN;
    }
    else if(dimensionStr.ends_with("pt"))
    {
        return CSSDimensionQualifier::DQ_PT;
    }
    else if(dimensionStr.ends_with("pc"))
    {
        return CSSDimensionQualifier::DQ_PC;
    }
    else if(dimensionStr.ends_with("q"))
    {
        return CSSDimensionQualifier::DQ_Q;
    }
    // ================================================================ angle
    else if(dimensionStr.ends_with("deg"))
    {
        return CSSDimensionQualifier::DQ_DEG;
    }
    else if(dimensionStr.ends_with("rad"))
    {
        return CSSDimensionQualifier::DQ_RAD;
    }
    else if(dimensionStr.ends_with("grad"))
    {
        return CSSDimensionQualifier::DQ_GRAD;
    }
    else if(dimensionStr.ends_with("turn"))
    {
        return CSSDimensionQualifier::DQ_TURN;
    }
    // ================================================================ time
    else if(dimensionStr.ends_with("ms"))
    {
        return CSSDimensionQualifier::DQ_MS;
    }
    else if(dimensionStr.ends_with("s"))
    {
        return CSSDimensionQualifier::DQ_S;
    }
    // ================================================================ frequency
    else if(dimensionStr.ends_with("hz"))
    {
        return CSSDimensionQualifier::DQ_HZ;
    }
    else if(dimensionStr.ends_with("khz"))
    {
        return CSSDimensionQualifier::DQ_KHZ;
    }
    // ================================================================ relative units
    else if(dimensionStr.ends_with("em"))
    {
        return CSSDimensionQualifier::DQ_EM;
    }
    else if(dimensionStr.ends_with("ex"))
    {
        return CSSDimensionQualifier::DQ_EX;
    }
    else if(dimensionStr.ends_with("ch"))
    {
        return CSSDimensionQualifier::DQ_CH;
    }
    else if(dimensionStr.ends_with("rem"))
    {
        return CSSDimensionQualifier::DQ_REM;
    }
    else if(dimensionStr.ends_with("vw"))
    {
        return CSSDimensionQualifier::DQ_VW;
    }
    else if(dimensionStr.ends_with("vh"))
    {
        return CSSDimensionQualifier::DQ_VH;
    }
    else if(dimensionStr.ends_with("vmin"))
    {
        return CSSDimensionQualifier::DQ_VMIN;
    }
    else if(dimensionStr.ends_with("vmax"))
    {
        return CSSDimensionQualifier::DQ_VMAX;
    }
    // ================================================================ resolution units
    else if(dimensionStr.ends_with("dpi"))
    {
        return CSSDimensionQualifier::DQ_DPI;
    }
    else if(dimensionStr.ends_with("dpcm"))
    {
        return CSSDimensionQualifier::DQ_DPCM;
    }
    else if(dimensionStr.ends_with("dppx"))
    {
        return CSSDimensionQualifier::DQ_DPPX;
    }
    #pragma endregion Units

    return CSSDimensionQualifier::DQ_PX;
}

