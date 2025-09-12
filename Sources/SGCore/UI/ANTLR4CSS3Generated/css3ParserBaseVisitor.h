
// Generated from /home/creastive/projects/sungear/SungearEngine/Externals/antlr4/css/css3Parser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "css3ParserVisitor.h"


/**
 * This class provides an empty implementation of css3ParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  css3ParserBaseVisitor : public css3ParserVisitor {
public:

  virtual std::any visitStylesheet(css3Parser::StylesheetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGoodCharset(css3Parser::GoodCharsetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBadCharset(css3Parser::BadCharsetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGoodImport(css3Parser::GoodImportContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBadImport(css3Parser::BadImportContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGoodNamespace(css3Parser::GoodNamespaceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBadNamespace(css3Parser::BadNamespaceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNamespacePrefix(css3Parser::NamespacePrefixContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMedia(css3Parser::MediaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMediaQueryList(css3Parser::MediaQueryListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMediaQuery(css3Parser::MediaQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMediaType(css3Parser::MediaTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMediaExpression(css3Parser::MediaExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMediaFeature(css3Parser::MediaFeatureContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPage(css3Parser::PageContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPseudoPage(css3Parser::PseudoPageContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelectorGroup(css3Parser::SelectorGroupContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelector(css3Parser::SelectorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCombinator(css3Parser::CombinatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSimpleSelectorSequence(css3Parser::SimpleSelectorSequenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTypeSelector(css3Parser::TypeSelectorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTypeNamespacePrefix(css3Parser::TypeNamespacePrefixContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitElementName(css3Parser::ElementNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUniversal(css3Parser::UniversalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassName(css3Parser::ClassNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttrib(css3Parser::AttribContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPseudo(css3Parser::PseudoContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionalPseudo(css3Parser::FunctionalPseudoContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpression(css3Parser::ExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNegation(css3Parser::NegationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNegationArg(css3Parser::NegationArgContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGoodOperator(css3Parser::GoodOperatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBadOperator(css3Parser::BadOperatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGoodProperty(css3Parser::GoodPropertyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBadProperty(css3Parser::BadPropertyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKnownRuleset(css3Parser::KnownRulesetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnknownRuleset(css3Parser::UnknownRulesetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclarationList(css3Parser::DeclarationListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKnownDeclaration(css3Parser::KnownDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnknownDeclaration(css3Parser::UnknownDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrio(css3Parser::PrioContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue(css3Parser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpr(css3Parser::ExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKnownTerm(css3Parser::KnownTermContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBadTerm(css3Parser::BadTermContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunction_(css3Parser::Function_Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDxImageTransform(css3Parser::DxImageTransformContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitHexcolor(css3Parser::HexcolorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber(css3Parser::NumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPercentage(css3Parser::PercentageContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDimension(css3Parser::DimensionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnknownDimension(css3Parser::UnknownDimensionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAny_(css3Parser::Any_Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnknownAtRule(css3Parser::UnknownAtRuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnused(css3Parser::UnusedContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlock(css3Parser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNestedStatement(css3Parser::NestedStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGroupRuleBody(css3Parser::GroupRuleBodyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSupportsRule(css3Parser::SupportsRuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSupportsCondition(css3Parser::SupportsConditionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSupportsConditionInParens(css3Parser::SupportsConditionInParensContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSupportsNegation(css3Parser::SupportsNegationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSupportsConjunction(css3Parser::SupportsConjunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSupportsDisjunction(css3Parser::SupportsDisjunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSupportsDeclarationCondition(css3Parser::SupportsDeclarationConditionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGeneralEnclosed(css3Parser::GeneralEnclosedContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUrl(css3Parser::UrlContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVar_(css3Parser::Var_Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCalc(css3Parser::CalcContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCalcOperand(css3Parser::CalcOperandContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCalcSign(css3Parser::CalcSignContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCalcExpr(css3Parser::CalcExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCalcNestedValue(css3Parser::CalcNestedValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCalcValue(css3Parser::CalcValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFontFaceRule(css3Parser::FontFaceRuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKnownFontFaceDeclaration(css3Parser::KnownFontFaceDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnknownFontFaceDeclaration(css3Parser::UnknownFontFaceDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKeyframesRule(css3Parser::KeyframesRuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKeyframeBlock(css3Parser::KeyframeBlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKeyframeSelector(css3Parser::KeyframeSelectorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitViewport(css3Parser::ViewportContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCounterStyle(css3Parser::CounterStyleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFontFeatureValuesRule(css3Parser::FontFeatureValuesRuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFontFamilyNameList(css3Parser::FontFamilyNameListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFontFamilyName(css3Parser::FontFamilyNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFeatureValueBlock(css3Parser::FeatureValueBlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFeatureType(css3Parser::FeatureTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFeatureValueDefinition(css3Parser::FeatureValueDefinitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIdent(css3Parser::IdentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWs(css3Parser::WsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColor(css3Parser::ColorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColor_alpha(css3Parser::Color_alphaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColor_component(css3Parser::Color_componentContext *ctx) override {
    return visitChildren(ctx);
  }


};

