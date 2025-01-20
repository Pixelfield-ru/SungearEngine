
// Generated from D:/Pixelfield/NativeSungearEngine/NativeSungearEngine/Externals/antlr4/css/css3Parser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "css3Parser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by css3Parser.
 */
class  css3ParserListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterStylesheet(css3Parser::StylesheetContext *ctx) = 0;
  virtual void exitStylesheet(css3Parser::StylesheetContext *ctx) = 0;

  virtual void enterGoodCharset(css3Parser::GoodCharsetContext *ctx) = 0;
  virtual void exitGoodCharset(css3Parser::GoodCharsetContext *ctx) = 0;

  virtual void enterBadCharset(css3Parser::BadCharsetContext *ctx) = 0;
  virtual void exitBadCharset(css3Parser::BadCharsetContext *ctx) = 0;

  virtual void enterGoodImport(css3Parser::GoodImportContext *ctx) = 0;
  virtual void exitGoodImport(css3Parser::GoodImportContext *ctx) = 0;

  virtual void enterBadImport(css3Parser::BadImportContext *ctx) = 0;
  virtual void exitBadImport(css3Parser::BadImportContext *ctx) = 0;

  virtual void enterGoodNamespace(css3Parser::GoodNamespaceContext *ctx) = 0;
  virtual void exitGoodNamespace(css3Parser::GoodNamespaceContext *ctx) = 0;

  virtual void enterBadNamespace(css3Parser::BadNamespaceContext *ctx) = 0;
  virtual void exitBadNamespace(css3Parser::BadNamespaceContext *ctx) = 0;

  virtual void enterNamespacePrefix(css3Parser::NamespacePrefixContext *ctx) = 0;
  virtual void exitNamespacePrefix(css3Parser::NamespacePrefixContext *ctx) = 0;

  virtual void enterMedia(css3Parser::MediaContext *ctx) = 0;
  virtual void exitMedia(css3Parser::MediaContext *ctx) = 0;

  virtual void enterMediaQueryList(css3Parser::MediaQueryListContext *ctx) = 0;
  virtual void exitMediaQueryList(css3Parser::MediaQueryListContext *ctx) = 0;

  virtual void enterMediaQuery(css3Parser::MediaQueryContext *ctx) = 0;
  virtual void exitMediaQuery(css3Parser::MediaQueryContext *ctx) = 0;

  virtual void enterMediaType(css3Parser::MediaTypeContext *ctx) = 0;
  virtual void exitMediaType(css3Parser::MediaTypeContext *ctx) = 0;

  virtual void enterMediaExpression(css3Parser::MediaExpressionContext *ctx) = 0;
  virtual void exitMediaExpression(css3Parser::MediaExpressionContext *ctx) = 0;

  virtual void enterMediaFeature(css3Parser::MediaFeatureContext *ctx) = 0;
  virtual void exitMediaFeature(css3Parser::MediaFeatureContext *ctx) = 0;

  virtual void enterPage(css3Parser::PageContext *ctx) = 0;
  virtual void exitPage(css3Parser::PageContext *ctx) = 0;

  virtual void enterPseudoPage(css3Parser::PseudoPageContext *ctx) = 0;
  virtual void exitPseudoPage(css3Parser::PseudoPageContext *ctx) = 0;

  virtual void enterSelectorGroup(css3Parser::SelectorGroupContext *ctx) = 0;
  virtual void exitSelectorGroup(css3Parser::SelectorGroupContext *ctx) = 0;

  virtual void enterSelector(css3Parser::SelectorContext *ctx) = 0;
  virtual void exitSelector(css3Parser::SelectorContext *ctx) = 0;

  virtual void enterCombinator(css3Parser::CombinatorContext *ctx) = 0;
  virtual void exitCombinator(css3Parser::CombinatorContext *ctx) = 0;

  virtual void enterSimpleSelectorSequence(css3Parser::SimpleSelectorSequenceContext *ctx) = 0;
  virtual void exitSimpleSelectorSequence(css3Parser::SimpleSelectorSequenceContext *ctx) = 0;

  virtual void enterTypeSelector(css3Parser::TypeSelectorContext *ctx) = 0;
  virtual void exitTypeSelector(css3Parser::TypeSelectorContext *ctx) = 0;

  virtual void enterTypeNamespacePrefix(css3Parser::TypeNamespacePrefixContext *ctx) = 0;
  virtual void exitTypeNamespacePrefix(css3Parser::TypeNamespacePrefixContext *ctx) = 0;

  virtual void enterElementName(css3Parser::ElementNameContext *ctx) = 0;
  virtual void exitElementName(css3Parser::ElementNameContext *ctx) = 0;

  virtual void enterUniversal(css3Parser::UniversalContext *ctx) = 0;
  virtual void exitUniversal(css3Parser::UniversalContext *ctx) = 0;

  virtual void enterClassName(css3Parser::ClassNameContext *ctx) = 0;
  virtual void exitClassName(css3Parser::ClassNameContext *ctx) = 0;

  virtual void enterAttrib(css3Parser::AttribContext *ctx) = 0;
  virtual void exitAttrib(css3Parser::AttribContext *ctx) = 0;

  virtual void enterPseudo(css3Parser::PseudoContext *ctx) = 0;
  virtual void exitPseudo(css3Parser::PseudoContext *ctx) = 0;

  virtual void enterFunctionalPseudo(css3Parser::FunctionalPseudoContext *ctx) = 0;
  virtual void exitFunctionalPseudo(css3Parser::FunctionalPseudoContext *ctx) = 0;

  virtual void enterExpression(css3Parser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(css3Parser::ExpressionContext *ctx) = 0;

  virtual void enterNegation(css3Parser::NegationContext *ctx) = 0;
  virtual void exitNegation(css3Parser::NegationContext *ctx) = 0;

  virtual void enterNegationArg(css3Parser::NegationArgContext *ctx) = 0;
  virtual void exitNegationArg(css3Parser::NegationArgContext *ctx) = 0;

  virtual void enterGoodOperator(css3Parser::GoodOperatorContext *ctx) = 0;
  virtual void exitGoodOperator(css3Parser::GoodOperatorContext *ctx) = 0;

  virtual void enterBadOperator(css3Parser::BadOperatorContext *ctx) = 0;
  virtual void exitBadOperator(css3Parser::BadOperatorContext *ctx) = 0;

  virtual void enterGoodProperty(css3Parser::GoodPropertyContext *ctx) = 0;
  virtual void exitGoodProperty(css3Parser::GoodPropertyContext *ctx) = 0;

  virtual void enterBadProperty(css3Parser::BadPropertyContext *ctx) = 0;
  virtual void exitBadProperty(css3Parser::BadPropertyContext *ctx) = 0;

  virtual void enterKnownRuleset(css3Parser::KnownRulesetContext *ctx) = 0;
  virtual void exitKnownRuleset(css3Parser::KnownRulesetContext *ctx) = 0;

  virtual void enterUnknownRuleset(css3Parser::UnknownRulesetContext *ctx) = 0;
  virtual void exitUnknownRuleset(css3Parser::UnknownRulesetContext *ctx) = 0;

  virtual void enterDeclarationList(css3Parser::DeclarationListContext *ctx) = 0;
  virtual void exitDeclarationList(css3Parser::DeclarationListContext *ctx) = 0;

  virtual void enterKnownDeclaration(css3Parser::KnownDeclarationContext *ctx) = 0;
  virtual void exitKnownDeclaration(css3Parser::KnownDeclarationContext *ctx) = 0;

  virtual void enterUnknownDeclaration(css3Parser::UnknownDeclarationContext *ctx) = 0;
  virtual void exitUnknownDeclaration(css3Parser::UnknownDeclarationContext *ctx) = 0;

  virtual void enterPrio(css3Parser::PrioContext *ctx) = 0;
  virtual void exitPrio(css3Parser::PrioContext *ctx) = 0;

  virtual void enterValue(css3Parser::ValueContext *ctx) = 0;
  virtual void exitValue(css3Parser::ValueContext *ctx) = 0;

  virtual void enterExpr(css3Parser::ExprContext *ctx) = 0;
  virtual void exitExpr(css3Parser::ExprContext *ctx) = 0;

  virtual void enterKnownTerm(css3Parser::KnownTermContext *ctx) = 0;
  virtual void exitKnownTerm(css3Parser::KnownTermContext *ctx) = 0;

  virtual void enterUnknownTerm(css3Parser::UnknownTermContext *ctx) = 0;
  virtual void exitUnknownTerm(css3Parser::UnknownTermContext *ctx) = 0;

  virtual void enterBadTerm(css3Parser::BadTermContext *ctx) = 0;
  virtual void exitBadTerm(css3Parser::BadTermContext *ctx) = 0;

  virtual void enterFunction_(css3Parser::Function_Context *ctx) = 0;
  virtual void exitFunction_(css3Parser::Function_Context *ctx) = 0;

  virtual void enterDxImageTransform(css3Parser::DxImageTransformContext *ctx) = 0;
  virtual void exitDxImageTransform(css3Parser::DxImageTransformContext *ctx) = 0;

  virtual void enterHexcolor(css3Parser::HexcolorContext *ctx) = 0;
  virtual void exitHexcolor(css3Parser::HexcolorContext *ctx) = 0;

  virtual void enterNumber(css3Parser::NumberContext *ctx) = 0;
  virtual void exitNumber(css3Parser::NumberContext *ctx) = 0;

  virtual void enterPercentage(css3Parser::PercentageContext *ctx) = 0;
  virtual void exitPercentage(css3Parser::PercentageContext *ctx) = 0;

  virtual void enterDimension(css3Parser::DimensionContext *ctx) = 0;
  virtual void exitDimension(css3Parser::DimensionContext *ctx) = 0;

  virtual void enterUnknownDimension(css3Parser::UnknownDimensionContext *ctx) = 0;
  virtual void exitUnknownDimension(css3Parser::UnknownDimensionContext *ctx) = 0;

  virtual void enterAny_(css3Parser::Any_Context *ctx) = 0;
  virtual void exitAny_(css3Parser::Any_Context *ctx) = 0;

  virtual void enterUnknownAtRule(css3Parser::UnknownAtRuleContext *ctx) = 0;
  virtual void exitUnknownAtRule(css3Parser::UnknownAtRuleContext *ctx) = 0;

  virtual void enterUnused(css3Parser::UnusedContext *ctx) = 0;
  virtual void exitUnused(css3Parser::UnusedContext *ctx) = 0;

  virtual void enterBlock(css3Parser::BlockContext *ctx) = 0;
  virtual void exitBlock(css3Parser::BlockContext *ctx) = 0;

  virtual void enterNestedStatement(css3Parser::NestedStatementContext *ctx) = 0;
  virtual void exitNestedStatement(css3Parser::NestedStatementContext *ctx) = 0;

  virtual void enterGroupRuleBody(css3Parser::GroupRuleBodyContext *ctx) = 0;
  virtual void exitGroupRuleBody(css3Parser::GroupRuleBodyContext *ctx) = 0;

  virtual void enterSupportsRule(css3Parser::SupportsRuleContext *ctx) = 0;
  virtual void exitSupportsRule(css3Parser::SupportsRuleContext *ctx) = 0;

  virtual void enterSupportsCondition(css3Parser::SupportsConditionContext *ctx) = 0;
  virtual void exitSupportsCondition(css3Parser::SupportsConditionContext *ctx) = 0;

  virtual void enterSupportsConditionInParens(css3Parser::SupportsConditionInParensContext *ctx) = 0;
  virtual void exitSupportsConditionInParens(css3Parser::SupportsConditionInParensContext *ctx) = 0;

  virtual void enterSupportsNegation(css3Parser::SupportsNegationContext *ctx) = 0;
  virtual void exitSupportsNegation(css3Parser::SupportsNegationContext *ctx) = 0;

  virtual void enterSupportsConjunction(css3Parser::SupportsConjunctionContext *ctx) = 0;
  virtual void exitSupportsConjunction(css3Parser::SupportsConjunctionContext *ctx) = 0;

  virtual void enterSupportsDisjunction(css3Parser::SupportsDisjunctionContext *ctx) = 0;
  virtual void exitSupportsDisjunction(css3Parser::SupportsDisjunctionContext *ctx) = 0;

  virtual void enterSupportsDeclarationCondition(css3Parser::SupportsDeclarationConditionContext *ctx) = 0;
  virtual void exitSupportsDeclarationCondition(css3Parser::SupportsDeclarationConditionContext *ctx) = 0;

  virtual void enterGeneralEnclosed(css3Parser::GeneralEnclosedContext *ctx) = 0;
  virtual void exitGeneralEnclosed(css3Parser::GeneralEnclosedContext *ctx) = 0;

  virtual void enterUrl(css3Parser::UrlContext *ctx) = 0;
  virtual void exitUrl(css3Parser::UrlContext *ctx) = 0;

  virtual void enterVar_(css3Parser::Var_Context *ctx) = 0;
  virtual void exitVar_(css3Parser::Var_Context *ctx) = 0;

  virtual void enterCalc(css3Parser::CalcContext *ctx) = 0;
  virtual void exitCalc(css3Parser::CalcContext *ctx) = 0;

  virtual void enterCalcOperand(css3Parser::CalcOperandContext *ctx) = 0;
  virtual void exitCalcOperand(css3Parser::CalcOperandContext *ctx) = 0;

  virtual void enterCalcExpr(css3Parser::CalcExprContext *ctx) = 0;
  virtual void exitCalcExpr(css3Parser::CalcExprContext *ctx) = 0;

  virtual void enterCalcNestedValue(css3Parser::CalcNestedValueContext *ctx) = 0;
  virtual void exitCalcNestedValue(css3Parser::CalcNestedValueContext *ctx) = 0;

  virtual void enterCalcValue(css3Parser::CalcValueContext *ctx) = 0;
  virtual void exitCalcValue(css3Parser::CalcValueContext *ctx) = 0;

  virtual void enterFontFaceRule(css3Parser::FontFaceRuleContext *ctx) = 0;
  virtual void exitFontFaceRule(css3Parser::FontFaceRuleContext *ctx) = 0;

  virtual void enterKnownFontFaceDeclaration(css3Parser::KnownFontFaceDeclarationContext *ctx) = 0;
  virtual void exitKnownFontFaceDeclaration(css3Parser::KnownFontFaceDeclarationContext *ctx) = 0;

  virtual void enterUnknownFontFaceDeclaration(css3Parser::UnknownFontFaceDeclarationContext *ctx) = 0;
  virtual void exitUnknownFontFaceDeclaration(css3Parser::UnknownFontFaceDeclarationContext *ctx) = 0;

  virtual void enterKeyframesRule(css3Parser::KeyframesRuleContext *ctx) = 0;
  virtual void exitKeyframesRule(css3Parser::KeyframesRuleContext *ctx) = 0;

  virtual void enterKeyframeBlock(css3Parser::KeyframeBlockContext *ctx) = 0;
  virtual void exitKeyframeBlock(css3Parser::KeyframeBlockContext *ctx) = 0;

  virtual void enterKeyframeSelector(css3Parser::KeyframeSelectorContext *ctx) = 0;
  virtual void exitKeyframeSelector(css3Parser::KeyframeSelectorContext *ctx) = 0;

  virtual void enterViewport(css3Parser::ViewportContext *ctx) = 0;
  virtual void exitViewport(css3Parser::ViewportContext *ctx) = 0;

  virtual void enterCounterStyle(css3Parser::CounterStyleContext *ctx) = 0;
  virtual void exitCounterStyle(css3Parser::CounterStyleContext *ctx) = 0;

  virtual void enterFontFeatureValuesRule(css3Parser::FontFeatureValuesRuleContext *ctx) = 0;
  virtual void exitFontFeatureValuesRule(css3Parser::FontFeatureValuesRuleContext *ctx) = 0;

  virtual void enterFontFamilyNameList(css3Parser::FontFamilyNameListContext *ctx) = 0;
  virtual void exitFontFamilyNameList(css3Parser::FontFamilyNameListContext *ctx) = 0;

  virtual void enterFontFamilyName(css3Parser::FontFamilyNameContext *ctx) = 0;
  virtual void exitFontFamilyName(css3Parser::FontFamilyNameContext *ctx) = 0;

  virtual void enterFeatureValueBlock(css3Parser::FeatureValueBlockContext *ctx) = 0;
  virtual void exitFeatureValueBlock(css3Parser::FeatureValueBlockContext *ctx) = 0;

  virtual void enterFeatureType(css3Parser::FeatureTypeContext *ctx) = 0;
  virtual void exitFeatureType(css3Parser::FeatureTypeContext *ctx) = 0;

  virtual void enterFeatureValueDefinition(css3Parser::FeatureValueDefinitionContext *ctx) = 0;
  virtual void exitFeatureValueDefinition(css3Parser::FeatureValueDefinitionContext *ctx) = 0;

  virtual void enterIdent(css3Parser::IdentContext *ctx) = 0;
  virtual void exitIdent(css3Parser::IdentContext *ctx) = 0;

  virtual void enterWs(css3Parser::WsContext *ctx) = 0;
  virtual void exitWs(css3Parser::WsContext *ctx) = 0;

  virtual void enterColor(css3Parser::ColorContext *ctx) = 0;
  virtual void exitColor(css3Parser::ColorContext *ctx) = 0;

  virtual void enterColor_alpha(css3Parser::Color_alphaContext *ctx) = 0;
  virtual void exitColor_alpha(css3Parser::Color_alphaContext *ctx) = 0;

  virtual void enterColor_component(css3Parser::Color_componentContext *ctx) = 0;
  virtual void exitColor_component(css3Parser::Color_componentContext *ctx) = 0;


};

