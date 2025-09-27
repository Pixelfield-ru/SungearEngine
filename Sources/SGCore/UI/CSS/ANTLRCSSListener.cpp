//
// Created by stuka on 18.01.2025.
//
#include "ANTLRCSSListener.h"
#include "CSSPropertyType.h"

#include "SGCore/Memory/AssetManager.h"

#include "PropertiesProcessors/CSSPropertyProcessor.h"
#include "PropertiesProcessors/CSSWidthPropertyProcessor.h"
#include "PropertiesProcessors/CSSHeightPropertyProcessor.h"
#include "PropertiesProcessors/CSSFlexDirectionPropertyProcessor.h"
#include "PropertiesProcessors/CSSDisplayPropertyProcessor.h"
#include "PropertiesProcessors/CSSBackgroundColorPropertyProcessor.h"
#include "PropertiesProcessors/CSSBorderRadiusPropertyProcessor.h"
#include "PropertiesProcessors/CSSGapPropertyProcessor.h"
#include "PropertiesProcessors/CSSPaddingPropertyProcessor.h"
#include "PropertiesProcessors/CSSFontFamilyPropertyProcessor.h"
#include "PropertiesProcessors/CSSFontSizePropertyProcessor.h"

void SGCore::UI::ANTLRCSSListener::enterSelector(css3Parser::SelectorContext* ctx)
{
    std::string currentSelectorName = "";
    std::string currentPseudoName = "";
    if(!ctx->children.empty())
    {
        if(!ctx->children[0]->children.empty())
        {
            if(ctx->children[0]->children[0]->children.size() > 1)
            {
                currentSelectorName = ctx->children[0]->children[0]->children[1]->getText();
            }
        }

        if(ctx->children[0]->children.size() > 1)
        {
            currentPseudoName = ctx->children[0]->children[1]->children[1]->getText();
        }
    }

    std::cout << "selector name: " << currentSelectorName << ", with pseudo: " << currentPseudoName << std::endl;

    auto assetManager = m_toCSSFile->getParentAssetManager();

    InterpolatedPath newSelectorPath = m_toCSSFile->getPath() / "selectors";
    // if selector is pseudo class
    if(!currentPseudoName.empty())
    {
        newSelectorPath /= currentSelectorName + ":" + currentPseudoName;

        auto pseudo = assetManager->getOrAddAssetByPath<CSSSelector>(newSelectorPath);

        pseudo->m_name = currentSelectorName;
        pseudo->m_pseudoName = currentSelectorName;

        m_currentPseudo = pseudo.get();
    }
    else
    {
        newSelectorPath /= currentSelectorName;

        auto foundSelector = m_toCSSFile->findSelector(currentSelectorName);

        if(foundSelector)
        {
            m_currentSelector = foundSelector.get();

            return;
        }

        m_toCSSFile->m_selectors.push_back(assetManager->getOrAddAssetByPath<CSSSelector>(newSelectorPath));

        m_currentSelector = m_toCSSFile->m_selectors.rbegin()->get();

        m_currentSelector->m_name = currentSelectorName;
    }
}

void SGCore::UI::ANTLRCSSListener::enterKnownDeclaration(css3Parser::KnownDeclarationContext* ctx)
{
    if(m_currentPseudo)
    {
        m_pseudosToResolve.push_back({ m_currentPseudo, ctx });
        m_currentPseudo = nullptr;
        return;
    }

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

    const CSSPropertyType propertyType = getPropertyTypeFromName(propertyName);

    switch(propertyType)
    {
        case CSSPropertyType::PT_DISPLAY:
        {
            CSSPropertyProcessor<CSSPropertyType::PT_DISPLAY>::processProperty(this,
                                                                               ctx,
                                                                               propertyName,
                                                                               propertyStringValue);

            break;
        }
        case CSSPropertyType::PT_FLEX_DIRECTION:
        {
            CSSPropertyProcessor<CSSPropertyType::PT_FLEX_DIRECTION>::processProperty(this,
                                                                                      ctx,
                                                                                      propertyName,
                                                                                      propertyStringValue);

            break;
        }
        case CSSPropertyType::PT_FLEX_WRAP:break;
        case CSSPropertyType::PT_JUSTIFY_CONTENT:break;
        case CSSPropertyType::PT_ALIGN_ITEMS:break;
        case CSSPropertyType::PT_ALIGN_CONTENT:break;
        case CSSPropertyType::PT_GAP:
        {
            CSSPropertyProcessor<CSSPropertyType::PT_GAP>::processProperty(this,
                            ctx,
                            propertyName,
                            propertyStringValue);

            break;
        }
        case CSSPropertyType::PT_ORDER:break;
        case CSSPropertyType::PT_FLEX_GROW:break;
        case CSSPropertyType::PT_FLEX_SHRINK:break;
        case CSSPropertyType::PT_WIDTH:
        {
            CSSPropertyProcessor<CSSPropertyType::PT_WIDTH>::processProperty(this,
                ctx,
                propertyName,
                propertyStringValue);

            break;
        }
        case CSSPropertyType::PT_HEIGHT:
        {
            CSSPropertyProcessor<CSSPropertyType::PT_HEIGHT>::processProperty(this,
                ctx,
                propertyName,
                propertyStringValue);

            break;
        }
        case CSSPropertyType::PT_BACKGROUND_COLOR:
        {
            CSSPropertyProcessor<CSSPropertyType::PT_BACKGROUND_COLOR>::processProperty(this,
                ctx,
                propertyName,
                propertyStringValue);

            break;
        }
        case CSSPropertyType::PT_BORDER_RADIUS:
        {
            CSSPropertyProcessor<CSSPropertyType::PT_BORDER_RADIUS>::processProperty(this,
                ctx,
                propertyName,
                propertyStringValue);

            break;
        }
        case CSSPropertyType::PT_PADDING:
        {
            CSSPropertyProcessor<CSSPropertyType::PT_PADDING>::processProperty(this,
                            ctx,
                            propertyName,
                            propertyStringValue);

            break;
        }
        case CSSPropertyType::PT_FONT_FAMILY:
        {
            CSSPropertyProcessor<CSSPropertyType::PT_FONT_FAMILY>::processProperty(this,
                            ctx,
                            propertyName,
                            propertyStringValue);

            break;
        }
        case CSSPropertyType::PT_FONT_SIZE:
        {
            CSSPropertyProcessor<CSSPropertyType::PT_FONT_SIZE>::processProperty(this,
                            ctx,
                            propertyName,
                            propertyStringValue);

            break;
        }
        case CSSPropertyType::PT_UNKNOWN:break;
    }
}

void SGCore::UI::ANTLRCSSListener::resolvePseudos() noexcept
{
    for(auto& [pseudo, decl] : m_pseudosToResolve)
    {
        auto selector = m_toCSSFile->findSelector(pseudo->m_name);
        if(!selector)
        {
            LOG_W(SGCORE_TAG, "SGCore::UI::ANTLRCSSListener::resolvePseudos: cannot resolve pseudo '{}' for selector '{}': this selector does not exist.", pseudo->m_pseudoName, pseudo->m_name);
            continue;
        }

        selector->copy(*pseudo);

        m_currentPseudo = nullptr;
        m_currentSelector = pseudo;

        enterKnownDeclaration(decl);
    }

    m_pseudosToResolve.clear();
}

void SGCore::UI::ANTLRCSSListener::processCalculation(antlr4::tree::ParseTree* currentANTLRNode,
                                                      const std::string& currentPropertyName,
                                                      const Ref<CSSMathNode>& currentParentMathNode,
                                                      const std::unordered_set<CSSDimensionQualifier>& supportedQualifiers) noexcept
{
    const bool isAllQualifiersSupported = supportedQualifiers.contains(CSSDimensionQualifier::DQ_ANY);

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

                    processCalculation(asExpr->calcOperand(i)->calcValue()->calcNestedValue(), currentPropertyName,
                                       newParentMathNode, supportedQualifiers);

                    continue;
                }
                if(asExpr->calcOperand(i)->calcValue()->number())
                {
                    auto newNumberNode = MakeRef<CSSMathNumericNode>();

                    newNumberNode->m_value = std::stof(asExpr->calcOperand(i)->calcValue()->number()->getText());
                    newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_NUMBER;

                    currentParentMathNode->m_operands.push_back(newNumberNode);

                    continue;
                }
                if(asExpr->calcOperand(i)->calcValue()->dimension())
                {
                    auto newNumberNode = MakeRef<CSSMathNumericNode>();

                    const std::string dimension = asExpr->calcOperand(i)->calcValue()->dimension()->Dimension()->getText();

                    newNumberNode->m_dimensionQualifier = getDimensionQualifierFromString(dimension);

                    if(!isAllQualifiersSupported &&
                       !supportedQualifiers.contains(newNumberNode->m_dimensionQualifier))
                    {
                        printUnsupportedQualifierInCurrentContextError(currentPropertyName,
                                                                       asExpr->calcOperand(i)->calcValue()->dimension()->Dimension()->getText(),
                                                                       supportedQualifiers);
                    }

                    // stof ignores chars after number so we dont care that we have 123px for example
                    newNumberNode->m_value = std::stof(dimension);

                    if(asExpr->calcOperand(i)->calcValue()->dimension()->Minus())
                    {
                        newNumberNode->m_value *= -1.0f;
                    }

                    currentParentMathNode->m_operands.push_back(newNumberNode);

                    continue;
                }
                if(asExpr->calcOperand(i)->calcValue()->unknownDimension())
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
                if(asExpr->calcOperand(i)->calcValue()->percentage())
                {
                    auto newNumberNode = MakeRef<CSSMathNumericNode>();

                    newNumberNode->m_value = std::stof(asExpr->calcOperand(i)->calcValue()->percentage()->Percentage()->getText());
                    newNumberNode->m_dimensionQualifier = CSSDimensionQualifier::DQ_PERCENT;

                    if(!isAllQualifiersSupported &&
                       !supportedQualifiers.contains(CSSDimensionQualifier::DQ_PERCENT))
                    {
                        printUnsupportedQualifierInCurrentContextError(currentPropertyName,
                                                                       asExpr->calcOperand(i)->calcValue()->percentage()->Percentage()->getText(),
                                                                       supportedQualifiers);
                    }

                    if(asExpr->calcOperand(i)->calcValue()->percentage()->Minus())
                    {
                        newNumberNode->m_value *= -1.0f;
                    }

                    currentParentMathNode->m_operands.push_back(newNumberNode);

                    continue;
                }
                if(asExpr->calcOperand(i)->calcValue()->calc())
                {
                    auto newParentMathNode = MakeRef<CSSMathNode>();

                    currentParentMathNode->m_operands.push_back(newParentMathNode);

                    processCalculation(asExpr->calcOperand(i)->calcValue()->calc(), currentPropertyName,
                                       newParentMathNode, supportedQualifiers);

                    continue;
                }
            }
        }
        else
        {
            processCalculation(child, currentPropertyName,
                               currentParentMathNode, supportedQualifiers);
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

void SGCore::UI::ANTLRCSSListener::printInvalidCountOfOperatorsInPropertyError(const std::string& propertyName,
                                                                               const std::string& defaultSetKeyword,
                                                                               const size_t& currentOperatorsCount,
                                                                               const std::int64_t& validOperatorsMinCount,
                                                                               const std::int64_t& validOperatorsMaxCount) const noexcept
{
    std::int64_t finalValidOperatorsMaxCount = validOperatorsMaxCount;

    if(finalValidOperatorsMaxCount < 0)
    {
        finalValidOperatorsMaxCount = validOperatorsMinCount;
    }

    LOG_E(SGCORE_TAG,
          "ANTLRCSSListener can not process property '{}' correctly: property has invalid count of operators in section 'value'. "
          "Property has been set to the default value. "
          "Count of operators: '{}'. "
          "Valid count of operators: (min: '{}', max: '{}'). "
          "Set keyword (default): '{}'.\n"
          "In CSS file: {}",
          propertyName,
          currentOperatorsCount,
          validOperatorsMinCount,
          finalValidOperatorsMaxCount,
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
          "Property has been set to the default value. "
          "Term index: '{}'. Term value: '{}'. "
          "Set keyword (default): '{}'.\n"
          "In CSS file: {}",
          propertyName,
          termIndex,
          termValue,
          defaultSetKeyword,
          Utils::toUTF8(m_toCSSFile->getPath().resolved().u16string()));
}

void SGCore::UI::ANTLRCSSListener::printBadOperatorInPropertyError(const std::string& propertyName,
                                                                   const std::int64_t& operatorIndex,
                                                                   const std::string& operatorValue,
                                                                   const std::string& defaultSetKeyword) const noexcept
{
    LOG_E(SGCORE_TAG,
          "ANTLRCSSListener can not process property '{}' correctly: property has invalid operator in section 'value'. "
          "Property has been set to the default value. "
          "Operator index: '{}'. Operator value: '{}'. "
          "Set keyword (default): '{}'.\n"
          "In CSS file: {}",
          propertyName,
          operatorIndex,
          operatorValue,
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

void SGCore::UI::ANTLRCSSListener::printUnsupportedQualifierInCurrentContextError(const std::string& propertyName,
                                                                                  const std::string& currentDimensionString,
                                                                                  const std::unordered_set<CSSDimensionQualifier>& supportedQualifiers) const noexcept
{
    std::string qualifiersString;

    if(supportedQualifiers.empty())
    {
        qualifiersString = "no supported qualifiers";
    }
    else
    {
        for(const auto& qualifier: supportedQualifiers)
        {
            qualifiersString += dimensionQualifierToString(qualifier) + ", ";
        }

        // erasing last ", " ...
        qualifiersString.erase(qualifiersString.size());
        qualifiersString.erase(qualifiersString.size() - 1);
    }

    LOG_E(SGCORE_TAG,
          "ANTLRCSSListener can not process property '{}' correctly: value of property has unsupported dimension qualifier in current context (context: '{}'). "
          "Property has been set to the default value. "
          "Current dimension: '{}'.\n"
          "Supported dimensions in current context: '{}'\n"
          "In CSS file: {}",
          propertyName,
          propertyName,
          currentDimensionString,
          qualifiersString,
          Utils::toUTF8(m_toCSSFile->getPath().resolved().u16string()));
}

void SGCore::UI::ANTLRCSSListener::printInvalidHexError(const std::string& propertyName,
                                                        const std::string& currentHex,
                                                        const std::string& defaultSetKeyword) const noexcept
{
    LOG_E(SGCORE_TAG,
          "ANTLRCSSListener can not process property '{}' correctly: property has unknown HEX in section 'value'. "
          "Property has been set to the default value. "
          "Current HEX: '{}'. "
          "Set keyword (default): '{}'.\n"
          "In CSS file: {}",
          propertyName,
          currentHex,
          defaultSetKeyword,
          Utils::toUTF8(m_toCSSFile->getPath().resolved().u16string()));
}
