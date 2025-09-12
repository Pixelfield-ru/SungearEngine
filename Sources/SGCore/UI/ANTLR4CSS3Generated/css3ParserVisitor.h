
// Generated from /home/creastive/projects/sungear/SungearEngine/Externals/antlr4/css/css3Parser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "css3Parser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by css3Parser.
 */
class  css3ParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by css3Parser.
   */
    virtual std::any visitStylesheet(css3Parser::StylesheetContext *context) = 0;

    virtual std::any visitGoodCharset(css3Parser::GoodCharsetContext *context) = 0;

    virtual std::any visitBadCharset(css3Parser::BadCharsetContext *context) = 0;

    virtual std::any visitGoodImport(css3Parser::GoodImportContext *context) = 0;

    virtual std::any visitBadImport(css3Parser::BadImportContext *context) = 0;

    virtual std::any visitGoodNamespace(css3Parser::GoodNamespaceContext *context) = 0;

    virtual std::any visitBadNamespace(css3Parser::BadNamespaceContext *context) = 0;

    virtual std::any visitNamespacePrefix(css3Parser::NamespacePrefixContext *context) = 0;

    virtual std::any visitMedia(css3Parser::MediaContext *context) = 0;

    virtual std::any visitMediaQueryList(css3Parser::MediaQueryListContext *context) = 0;

    virtual std::any visitMediaQuery(css3Parser::MediaQueryContext *context) = 0;

    virtual std::any visitMediaType(css3Parser::MediaTypeContext *context) = 0;

    virtual std::any visitMediaExpression(css3Parser::MediaExpressionContext *context) = 0;

    virtual std::any visitMediaFeature(css3Parser::MediaFeatureContext *context) = 0;

    virtual std::any visitPage(css3Parser::PageContext *context) = 0;

    virtual std::any visitPseudoPage(css3Parser::PseudoPageContext *context) = 0;

    virtual std::any visitSelectorGroup(css3Parser::SelectorGroupContext *context) = 0;

    virtual std::any visitSelector(css3Parser::SelectorContext *context) = 0;

    virtual std::any visitCombinator(css3Parser::CombinatorContext *context) = 0;

    virtual std::any visitSimpleSelectorSequence(css3Parser::SimpleSelectorSequenceContext *context) = 0;

    virtual std::any visitTypeSelector(css3Parser::TypeSelectorContext *context) = 0;

    virtual std::any visitTypeNamespacePrefix(css3Parser::TypeNamespacePrefixContext *context) = 0;

    virtual std::any visitElementName(css3Parser::ElementNameContext *context) = 0;

    virtual std::any visitUniversal(css3Parser::UniversalContext *context) = 0;

    virtual std::any visitClassName(css3Parser::ClassNameContext *context) = 0;

    virtual std::any visitAttrib(css3Parser::AttribContext *context) = 0;

    virtual std::any visitPseudo(css3Parser::PseudoContext *context) = 0;

    virtual std::any visitFunctionalPseudo(css3Parser::FunctionalPseudoContext *context) = 0;

    virtual std::any visitExpression(css3Parser::ExpressionContext *context) = 0;

    virtual std::any visitNegation(css3Parser::NegationContext *context) = 0;

    virtual std::any visitNegationArg(css3Parser::NegationArgContext *context) = 0;

    virtual std::any visitGoodOperator(css3Parser::GoodOperatorContext *context) = 0;

    virtual std::any visitBadOperator(css3Parser::BadOperatorContext *context) = 0;

    virtual std::any visitGoodProperty(css3Parser::GoodPropertyContext *context) = 0;

    virtual std::any visitBadProperty(css3Parser::BadPropertyContext *context) = 0;

    virtual std::any visitKnownRuleset(css3Parser::KnownRulesetContext *context) = 0;

    virtual std::any visitUnknownRuleset(css3Parser::UnknownRulesetContext *context) = 0;

    virtual std::any visitDeclarationList(css3Parser::DeclarationListContext *context) = 0;

    virtual std::any visitKnownDeclaration(css3Parser::KnownDeclarationContext *context) = 0;

    virtual std::any visitUnknownDeclaration(css3Parser::UnknownDeclarationContext *context) = 0;

    virtual std::any visitPrio(css3Parser::PrioContext *context) = 0;

    virtual std::any visitValue(css3Parser::ValueContext *context) = 0;

    virtual std::any visitExpr(css3Parser::ExprContext *context) = 0;

    virtual std::any visitKnownTerm(css3Parser::KnownTermContext *context) = 0;

    virtual std::any visitBadTerm(css3Parser::BadTermContext *context) = 0;

    virtual std::any visitFunction_(css3Parser::Function_Context *context) = 0;

    virtual std::any visitDxImageTransform(css3Parser::DxImageTransformContext *context) = 0;

    virtual std::any visitHexcolor(css3Parser::HexcolorContext *context) = 0;

    virtual std::any visitNumber(css3Parser::NumberContext *context) = 0;

    virtual std::any visitPercentage(css3Parser::PercentageContext *context) = 0;

    virtual std::any visitDimension(css3Parser::DimensionContext *context) = 0;

    virtual std::any visitUnknownDimension(css3Parser::UnknownDimensionContext *context) = 0;

    virtual std::any visitAny_(css3Parser::Any_Context *context) = 0;

    virtual std::any visitUnknownAtRule(css3Parser::UnknownAtRuleContext *context) = 0;

    virtual std::any visitUnused(css3Parser::UnusedContext *context) = 0;

    virtual std::any visitBlock(css3Parser::BlockContext *context) = 0;

    virtual std::any visitNestedStatement(css3Parser::NestedStatementContext *context) = 0;

    virtual std::any visitGroupRuleBody(css3Parser::GroupRuleBodyContext *context) = 0;

    virtual std::any visitSupportsRule(css3Parser::SupportsRuleContext *context) = 0;

    virtual std::any visitSupportsCondition(css3Parser::SupportsConditionContext *context) = 0;

    virtual std::any visitSupportsConditionInParens(css3Parser::SupportsConditionInParensContext *context) = 0;

    virtual std::any visitSupportsNegation(css3Parser::SupportsNegationContext *context) = 0;

    virtual std::any visitSupportsConjunction(css3Parser::SupportsConjunctionContext *context) = 0;

    virtual std::any visitSupportsDisjunction(css3Parser::SupportsDisjunctionContext *context) = 0;

    virtual std::any visitSupportsDeclarationCondition(css3Parser::SupportsDeclarationConditionContext *context) = 0;

    virtual std::any visitGeneralEnclosed(css3Parser::GeneralEnclosedContext *context) = 0;

    virtual std::any visitUrl(css3Parser::UrlContext *context) = 0;

    virtual std::any visitVar_(css3Parser::Var_Context *context) = 0;

    virtual std::any visitCalc(css3Parser::CalcContext *context) = 0;

    virtual std::any visitCalcOperand(css3Parser::CalcOperandContext *context) = 0;

    virtual std::any visitCalcSign(css3Parser::CalcSignContext *context) = 0;

    virtual std::any visitCalcExpr(css3Parser::CalcExprContext *context) = 0;

    virtual std::any visitCalcNestedValue(css3Parser::CalcNestedValueContext *context) = 0;

    virtual std::any visitCalcValue(css3Parser::CalcValueContext *context) = 0;

    virtual std::any visitFontFaceRule(css3Parser::FontFaceRuleContext *context) = 0;

    virtual std::any visitKnownFontFaceDeclaration(css3Parser::KnownFontFaceDeclarationContext *context) = 0;

    virtual std::any visitUnknownFontFaceDeclaration(css3Parser::UnknownFontFaceDeclarationContext *context) = 0;

    virtual std::any visitKeyframesRule(css3Parser::KeyframesRuleContext *context) = 0;

    virtual std::any visitKeyframeBlock(css3Parser::KeyframeBlockContext *context) = 0;

    virtual std::any visitKeyframeSelector(css3Parser::KeyframeSelectorContext *context) = 0;

    virtual std::any visitViewport(css3Parser::ViewportContext *context) = 0;

    virtual std::any visitCounterStyle(css3Parser::CounterStyleContext *context) = 0;

    virtual std::any visitFontFeatureValuesRule(css3Parser::FontFeatureValuesRuleContext *context) = 0;

    virtual std::any visitFontFamilyNameList(css3Parser::FontFamilyNameListContext *context) = 0;

    virtual std::any visitFontFamilyName(css3Parser::FontFamilyNameContext *context) = 0;

    virtual std::any visitFeatureValueBlock(css3Parser::FeatureValueBlockContext *context) = 0;

    virtual std::any visitFeatureType(css3Parser::FeatureTypeContext *context) = 0;

    virtual std::any visitFeatureValueDefinition(css3Parser::FeatureValueDefinitionContext *context) = 0;

    virtual std::any visitIdent(css3Parser::IdentContext *context) = 0;

    virtual std::any visitWs(css3Parser::WsContext *context) = 0;

    virtual std::any visitColor(css3Parser::ColorContext *context) = 0;

    virtual std::any visitColor_alpha(css3Parser::Color_alphaContext *context) = 0;

    virtual std::any visitColor_component(css3Parser::Color_componentContext *context) = 0;


};

