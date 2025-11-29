
// Generated from D:/dev/projects/SungearProjects/SungearEngine/Externals/antlr4/css/css3Parser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  css3Parser : public antlr4::Parser {
public:
  enum {
    OpenBracket = 1, CloseBracket = 2, OpenParen = 3, CloseParen = 4, OpenBrace = 5, 
    CloseBrace = 6, SemiColon = 7, Equal = 8, Colon = 9, Dot = 10, Multiply = 11, 
    Divide = 12, Pipe = 13, Underscore = 14, Comment = 15, Url = 16, Space = 17, 
    Cdo = 18, Cdc = 19, Includes = 20, DashMatch = 21, Hash = 22, Import = 23, 
    Page = 24, Media = 25, Namespace = 26, Charset = 27, Important = 28, 
    Percentage = 29, Url_ = 30, UnicodeRange = 31, MediaOnly = 32, Not = 33, 
    And = 34, Dimension = 35, UnknownDimension = 36, Plus = 37, Minus = 38, 
    Greater = 39, Comma = 40, Tilde = 41, PseudoNot = 42, Number = 43, String_ = 44, 
    PrefixMatch = 45, SuffixMatch = 46, SubstringMatch = 47, FontFace = 48, 
    Supports = 49, Or = 50, Keyframes = 51, From = 52, To = 53, Calc = 54, 
    Rgb = 55, Rgba = 56, Viewport = 57, CounterStyle = 58, FontFeatureValues = 59, 
    DxImageTransform = 60, AtKeyword = 61, Variable = 62, Var = 63, Ident = 64, 
    Function_ = 65, UnexpectedCharacter = 66
  };

  enum {
    RuleStylesheet = 0, RuleCharset = 1, RuleImports = 2, RuleNamespace_ = 3, 
    RuleNamespacePrefix = 4, RuleMedia = 5, RuleMediaQueryList = 6, RuleMediaQuery = 7, 
    RuleMediaType = 8, RuleMediaExpression = 9, RuleMediaFeature = 10, RulePage = 11, 
    RulePseudoPage = 12, RuleSelectorGroup = 13, RuleSelector = 14, RuleCombinator = 15, 
    RuleSimpleSelectorSequence = 16, RuleTypeSelector = 17, RuleTypeNamespacePrefix = 18, 
    RuleElementName = 19, RuleUniversal = 20, RuleClassName = 21, RuleAttrib = 22, 
    RulePseudo = 23, RuleFunctionalPseudo = 24, RuleExpression = 25, RuleNegation = 26, 
    RuleNegationArg = 27, RuleOperator_ = 28, RuleProperty_ = 29, RuleRuleset = 30, 
    RuleDeclarationList = 31, RuleDeclaration = 32, RulePrio = 33, RuleValue = 34, 
    RuleExpr = 35, RuleTerm = 36, RuleFunction_ = 37, RuleDxImageTransform = 38, 
    RuleHexcolor = 39, RuleNumber = 40, RulePercentage = 41, RuleDimension = 42, 
    RuleUnknownDimension = 43, RuleAny_ = 44, RuleAtRule = 45, RuleUnused = 46, 
    RuleBlock = 47, RuleNestedStatement = 48, RuleGroupRuleBody = 49, RuleSupportsRule = 50, 
    RuleSupportsCondition = 51, RuleSupportsConditionInParens = 52, RuleSupportsNegation = 53, 
    RuleSupportsConjunction = 54, RuleSupportsDisjunction = 55, RuleSupportsDeclarationCondition = 56, 
    RuleGeneralEnclosed = 57, RuleUrl = 58, RuleVar_ = 59, RuleCalc = 60, 
    RuleCalcOperand = 61, RuleCalcSign = 62, RuleCalcExpr = 63, RuleCalcNestedValue = 64, 
    RuleCalcValue = 65, RuleFontFaceRule = 66, RuleFontFaceDeclaration = 67, 
    RuleKeyframesRule = 68, RuleKeyframeBlock = 69, RuleKeyframeSelector = 70, 
    RuleViewport = 71, RuleCounterStyle = 72, RuleFontFeatureValuesRule = 73, 
    RuleFontFamilyNameList = 74, RuleFontFamilyName = 75, RuleFeatureValueBlock = 76, 
    RuleFeatureType = 77, RuleFeatureValueDefinition = 78, RuleIdent = 79, 
    RuleWs = 80, RuleColor = 81, RuleColor_alpha = 82, RuleColor_component = 83
  };

  explicit css3Parser(antlr4::TokenStream *input);

  css3Parser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~css3Parser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class StylesheetContext;
  class CharsetContext;
  class ImportsContext;
  class Namespace_Context;
  class NamespacePrefixContext;
  class MediaContext;
  class MediaQueryListContext;
  class MediaQueryContext;
  class MediaTypeContext;
  class MediaExpressionContext;
  class MediaFeatureContext;
  class PageContext;
  class PseudoPageContext;
  class SelectorGroupContext;
  class SelectorContext;
  class CombinatorContext;
  class SimpleSelectorSequenceContext;
  class TypeSelectorContext;
  class TypeNamespacePrefixContext;
  class ElementNameContext;
  class UniversalContext;
  class ClassNameContext;
  class AttribContext;
  class PseudoContext;
  class FunctionalPseudoContext;
  class ExpressionContext;
  class NegationContext;
  class NegationArgContext;
  class Operator_Context;
  class Property_Context;
  class RulesetContext;
  class DeclarationListContext;
  class DeclarationContext;
  class PrioContext;
  class ValueContext;
  class ExprContext;
  class TermContext;
  class Function_Context;
  class DxImageTransformContext;
  class HexcolorContext;
  class NumberContext;
  class PercentageContext;
  class DimensionContext;
  class UnknownDimensionContext;
  class Any_Context;
  class AtRuleContext;
  class UnusedContext;
  class BlockContext;
  class NestedStatementContext;
  class GroupRuleBodyContext;
  class SupportsRuleContext;
  class SupportsConditionContext;
  class SupportsConditionInParensContext;
  class SupportsNegationContext;
  class SupportsConjunctionContext;
  class SupportsDisjunctionContext;
  class SupportsDeclarationConditionContext;
  class GeneralEnclosedContext;
  class UrlContext;
  class Var_Context;
  class CalcContext;
  class CalcOperandContext;
  class CalcSignContext;
  class CalcExprContext;
  class CalcNestedValueContext;
  class CalcValueContext;
  class FontFaceRuleContext;
  class FontFaceDeclarationContext;
  class KeyframesRuleContext;
  class KeyframeBlockContext;
  class KeyframeSelectorContext;
  class ViewportContext;
  class CounterStyleContext;
  class FontFeatureValuesRuleContext;
  class FontFamilyNameListContext;
  class FontFamilyNameContext;
  class FeatureValueBlockContext;
  class FeatureTypeContext;
  class FeatureValueDefinitionContext;
  class IdentContext;
  class WsContext;
  class ColorContext;
  class Color_alphaContext;
  class Color_componentContext; 

  class  StylesheetContext : public antlr4::ParserRuleContext {
  public:
    StylesheetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    WsContext *ws();
    antlr4::tree::TerminalNode *EOF();
    std::vector<CharsetContext *> charset();
    CharsetContext* charset(size_t i);
    std::vector<ImportsContext *> imports();
    ImportsContext* imports(size_t i);
    std::vector<Namespace_Context *> namespace_();
    Namespace_Context* namespace_(size_t i);
    std::vector<NestedStatementContext *> nestedStatement();
    NestedStatementContext* nestedStatement(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comment();
    antlr4::tree::TerminalNode* Comment(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Space();
    antlr4::tree::TerminalNode* Space(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Cdo();
    antlr4::tree::TerminalNode* Cdo(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Cdc();
    antlr4::tree::TerminalNode* Cdc(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StylesheetContext* stylesheet();

  class  CharsetContext : public antlr4::ParserRuleContext {
  public:
    CharsetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    CharsetContext() = default;
    void copyFrom(CharsetContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  BadCharsetContext : public CharsetContext {
  public:
    BadCharsetContext(CharsetContext *ctx);

    antlr4::tree::TerminalNode *Charset();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *String_();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GoodCharsetContext : public CharsetContext {
  public:
    GoodCharsetContext(CharsetContext *ctx);

    antlr4::tree::TerminalNode *Charset();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *String_();
    antlr4::tree::TerminalNode *SemiColon();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  CharsetContext* charset();

  class  ImportsContext : public antlr4::ParserRuleContext {
  public:
    ImportsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ImportsContext() = default;
    void copyFrom(ImportsContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  BadImportContext : public ImportsContext {
  public:
    BadImportContext(ImportsContext *ctx);

    antlr4::tree::TerminalNode *Import();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    MediaQueryListContext *mediaQueryList();
    antlr4::tree::TerminalNode *String_();
    UrlContext *url();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GoodImportContext : public ImportsContext {
  public:
    GoodImportContext(ImportsContext *ctx);

    antlr4::tree::TerminalNode *Import();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    MediaQueryListContext *mediaQueryList();
    antlr4::tree::TerminalNode *SemiColon();
    antlr4::tree::TerminalNode *String_();
    UrlContext *url();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ImportsContext* imports();

  class  Namespace_Context : public antlr4::ParserRuleContext {
  public:
    Namespace_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Namespace_Context() = default;
    void copyFrom(Namespace_Context *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  GoodNamespaceContext : public Namespace_Context {
  public:
    GoodNamespaceContext(Namespace_Context *ctx);

    antlr4::tree::TerminalNode *Namespace();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *SemiColon();
    antlr4::tree::TerminalNode *String_();
    UrlContext *url();
    NamespacePrefixContext *namespacePrefix();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BadNamespaceContext : public Namespace_Context {
  public:
    BadNamespaceContext(Namespace_Context *ctx);

    antlr4::tree::TerminalNode *Namespace();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *String_();
    UrlContext *url();
    NamespacePrefixContext *namespacePrefix();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Namespace_Context* namespace_();

  class  NamespacePrefixContext : public antlr4::ParserRuleContext {
  public:
    NamespacePrefixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NamespacePrefixContext* namespacePrefix();

  class  MediaContext : public antlr4::ParserRuleContext {
  public:
    MediaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Media();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    MediaQueryListContext *mediaQueryList();
    GroupRuleBodyContext *groupRuleBody();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MediaContext* media();

  class  MediaQueryListContext : public antlr4::ParserRuleContext {
  public:
    MediaQueryListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    std::vector<MediaQueryContext *> mediaQuery();
    MediaQueryContext* mediaQuery(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MediaQueryListContext* mediaQueryList();

  class  MediaQueryContext : public antlr4::ParserRuleContext {
  public:
    MediaQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    MediaTypeContext *mediaType();
    std::vector<antlr4::tree::TerminalNode *> And();
    antlr4::tree::TerminalNode* And(size_t i);
    std::vector<MediaExpressionContext *> mediaExpression();
    MediaExpressionContext* mediaExpression(size_t i);
    antlr4::tree::TerminalNode *MediaOnly();
    antlr4::tree::TerminalNode *Not();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MediaQueryContext* mediaQuery();

  class  MediaTypeContext : public antlr4::ParserRuleContext {
  public:
    MediaTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MediaTypeContext* mediaType();

  class  MediaExpressionContext : public antlr4::ParserRuleContext {
  public:
    MediaExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OpenParen();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    MediaFeatureContext *mediaFeature();
    antlr4::tree::TerminalNode *CloseParen();
    antlr4::tree::TerminalNode *Colon();
    ExprContext *expr();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MediaExpressionContext* mediaExpression();

  class  MediaFeatureContext : public antlr4::ParserRuleContext {
  public:
    MediaFeatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    WsContext *ws();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MediaFeatureContext* mediaFeature();

  class  PageContext : public antlr4::ParserRuleContext {
  public:
    PageContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Page();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *OpenBrace();
    antlr4::tree::TerminalNode *CloseBrace();
    PseudoPageContext *pseudoPage();
    std::vector<DeclarationContext *> declaration();
    DeclarationContext* declaration(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SemiColon();
    antlr4::tree::TerminalNode* SemiColon(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PageContext* page();

  class  PseudoPageContext : public antlr4::ParserRuleContext {
  public:
    PseudoPageContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Colon();
    IdentContext *ident();
    WsContext *ws();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PseudoPageContext* pseudoPage();

  class  SelectorGroupContext : public antlr4::ParserRuleContext {
  public:
    SelectorGroupContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SelectorContext *> selector();
    SelectorContext* selector(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectorGroupContext* selectorGroup();

  class  SelectorContext : public antlr4::ParserRuleContext {
  public:
    SelectorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SimpleSelectorSequenceContext *> simpleSelectorSequence();
    SimpleSelectorSequenceContext* simpleSelectorSequence(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    std::vector<CombinatorContext *> combinator();
    CombinatorContext* combinator(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectorContext* selector();

  class  CombinatorContext : public antlr4::ParserRuleContext {
  public:
    CombinatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Plus();
    WsContext *ws();
    antlr4::tree::TerminalNode *Greater();
    antlr4::tree::TerminalNode *Tilde();
    antlr4::tree::TerminalNode *Space();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CombinatorContext* combinator();

  class  SimpleSelectorSequenceContext : public antlr4::ParserRuleContext {
  public:
    SimpleSelectorSequenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeSelectorContext *typeSelector();
    UniversalContext *universal();
    std::vector<antlr4::tree::TerminalNode *> Hash();
    antlr4::tree::TerminalNode* Hash(size_t i);
    std::vector<ClassNameContext *> className();
    ClassNameContext* className(size_t i);
    std::vector<AttribContext *> attrib();
    AttribContext* attrib(size_t i);
    std::vector<PseudoContext *> pseudo();
    PseudoContext* pseudo(size_t i);
    std::vector<NegationContext *> negation();
    NegationContext* negation(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleSelectorSequenceContext* simpleSelectorSequence();

  class  TypeSelectorContext : public antlr4::ParserRuleContext {
  public:
    TypeSelectorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementNameContext *elementName();
    TypeNamespacePrefixContext *typeNamespacePrefix();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeSelectorContext* typeSelector();

  class  TypeNamespacePrefixContext : public antlr4::ParserRuleContext {
  public:
    TypeNamespacePrefixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Pipe();
    IdentContext *ident();
    antlr4::tree::TerminalNode *Multiply();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeNamespacePrefixContext* typeNamespacePrefix();

  class  ElementNameContext : public antlr4::ParserRuleContext {
  public:
    ElementNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementNameContext* elementName();

  class  UniversalContext : public antlr4::ParserRuleContext {
  public:
    UniversalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Multiply();
    TypeNamespacePrefixContext *typeNamespacePrefix();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UniversalContext* universal();

  class  ClassNameContext : public antlr4::ParserRuleContext {
  public:
    ClassNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Dot();
    IdentContext *ident();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassNameContext* className();

  class  AttribContext : public antlr4::ParserRuleContext {
  public:
    AttribContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OpenBracket();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    std::vector<IdentContext *> ident();
    IdentContext* ident(size_t i);
    antlr4::tree::TerminalNode *CloseBracket();
    TypeNamespacePrefixContext *typeNamespacePrefix();
    antlr4::tree::TerminalNode *PrefixMatch();
    antlr4::tree::TerminalNode *SuffixMatch();
    antlr4::tree::TerminalNode *SubstringMatch();
    antlr4::tree::TerminalNode *Equal();
    antlr4::tree::TerminalNode *Includes();
    antlr4::tree::TerminalNode *DashMatch();
    antlr4::tree::TerminalNode *String_();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AttribContext* attrib();

  class  PseudoContext : public antlr4::ParserRuleContext {
  public:
    PseudoContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> Colon();
    antlr4::tree::TerminalNode* Colon(size_t i);
    IdentContext *ident();
    FunctionalPseudoContext *functionalPseudo();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PseudoContext* pseudo();

  class  FunctionalPseudoContext : public antlr4::ParserRuleContext {
  public:
    FunctionalPseudoContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Function_();
    WsContext *ws();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *CloseParen();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionalPseudoContext* functionalPseudo();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Plus();
    antlr4::tree::TerminalNode* Plus(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Minus();
    antlr4::tree::TerminalNode* Minus(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Dimension();
    antlr4::tree::TerminalNode* Dimension(size_t i);
    std::vector<antlr4::tree::TerminalNode *> UnknownDimension();
    antlr4::tree::TerminalNode* UnknownDimension(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Number();
    antlr4::tree::TerminalNode* Number(size_t i);
    std::vector<antlr4::tree::TerminalNode *> String_();
    antlr4::tree::TerminalNode* String_(size_t i);
    std::vector<IdentContext *> ident();
    IdentContext* ident(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExpressionContext* expression();

  class  NegationContext : public antlr4::ParserRuleContext {
  public:
    NegationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PseudoNot();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    NegationArgContext *negationArg();
    antlr4::tree::TerminalNode *CloseParen();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NegationContext* negation();

  class  NegationArgContext : public antlr4::ParserRuleContext {
  public:
    NegationArgContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeSelectorContext *typeSelector();
    UniversalContext *universal();
    antlr4::tree::TerminalNode *Hash();
    ClassNameContext *className();
    AttribContext *attrib();
    PseudoContext *pseudo();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NegationArgContext* negationArg();

  class  Operator_Context : public antlr4::ParserRuleContext {
  public:
    Operator_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Operator_Context() = default;
    void copyFrom(Operator_Context *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  BadOperatorContext : public Operator_Context {
  public:
    BadOperatorContext(Operator_Context *ctx);

    antlr4::tree::TerminalNode *Equal();
    WsContext *ws();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GoodOperatorContext : public Operator_Context {
  public:
    GoodOperatorContext(Operator_Context *ctx);

    antlr4::tree::TerminalNode *Divide();
    WsContext *ws();
    antlr4::tree::TerminalNode *Comma();
    antlr4::tree::TerminalNode *Space();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Operator_Context* operator_();

  class  Property_Context : public antlr4::ParserRuleContext {
  public:
    Property_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Property_Context() = default;
    void copyFrom(Property_Context *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  BadPropertyContext : public Property_Context {
  public:
    BadPropertyContext(Property_Context *ctx);

    antlr4::tree::TerminalNode *Multiply();
    IdentContext *ident();
    antlr4::tree::TerminalNode *Underscore();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GoodPropertyContext : public Property_Context {
  public:
    GoodPropertyContext(Property_Context *ctx);

    IdentContext *ident();
    WsContext *ws();
    antlr4::tree::TerminalNode *Variable();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Property_Context* property_();

  class  RulesetContext : public antlr4::ParserRuleContext {
  public:
    RulesetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    RulesetContext() = default;
    void copyFrom(RulesetContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  UnknownRulesetContext : public RulesetContext {
  public:
    UnknownRulesetContext(RulesetContext *ctx);

    antlr4::tree::TerminalNode *OpenBrace();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *CloseBrace();
    std::vector<Any_Context *> any_();
    Any_Context* any_(size_t i);
    DeclarationListContext *declarationList();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  KnownRulesetContext : public RulesetContext {
  public:
    KnownRulesetContext(RulesetContext *ctx);

    SelectorGroupContext *selectorGroup();
    antlr4::tree::TerminalNode *OpenBrace();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *CloseBrace();
    DeclarationListContext *declarationList();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  RulesetContext* ruleset();

  class  DeclarationListContext : public antlr4::ParserRuleContext {
  public:
    DeclarationListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DeclarationContext *> declaration();
    DeclarationContext* declaration(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SemiColon();
    antlr4::tree::TerminalNode* SemiColon(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclarationListContext* declarationList();

  class  DeclarationContext : public antlr4::ParserRuleContext {
  public:
    DeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    DeclarationContext() = default;
    void copyFrom(DeclarationContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  UnknownDeclarationContext : public DeclarationContext {
  public:
    UnknownDeclarationContext(DeclarationContext *ctx);

    Property_Context *property_();
    antlr4::tree::TerminalNode *Colon();
    WsContext *ws();
    ValueContext *value();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  KnownDeclarationContext : public DeclarationContext {
  public:
    KnownDeclarationContext(DeclarationContext *ctx);

    Property_Context *property_();
    antlr4::tree::TerminalNode *Colon();
    WsContext *ws();
    ExprContext *expr();
    PrioContext *prio();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  DeclarationContext* declaration();

  class  PrioContext : public antlr4::ParserRuleContext {
  public:
    PrioContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Important();
    WsContext *ws();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrioContext* prio();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Any_Context *> any_();
    Any_Context* any_(size_t i);
    std::vector<BlockContext *> block();
    BlockContext* block(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AtKeyword();
    antlr4::tree::TerminalNode* AtKeyword(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueContext* value();

  class  ExprContext : public antlr4::ParserRuleContext {
  public:
    ExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TermContext *> term();
    TermContext* term(size_t i);
    std::vector<Operator_Context *> operator_();
    Operator_Context* operator_(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExprContext* expr();

  class  TermContext : public antlr4::ParserRuleContext {
  public:
    TermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    TermContext() = default;
    void copyFrom(TermContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  BadTermContext : public TermContext {
  public:
    BadTermContext(TermContext *ctx);

    UnknownDimensionContext *unknownDimension();
    WsContext *ws();
    DxImageTransformContext *dxImageTransform();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  KnownTermContext : public TermContext {
  public:
    KnownTermContext(TermContext *ctx);

    NumberContext *number();
    WsContext *ws();
    PercentageContext *percentage();
    DimensionContext *dimension();
    antlr4::tree::TerminalNode *String_();
    antlr4::tree::TerminalNode *UnicodeRange();
    IdentContext *ident();
    Var_Context *var_();
    UrlContext *url();
    HexcolorContext *hexcolor();
    CalcContext *calc();
    Function_Context *function_();
    ColorContext *color();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  TermContext* term();

  class  Function_Context : public antlr4::ParserRuleContext {
  public:
    Function_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Function_();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    ExprContext *expr();
    antlr4::tree::TerminalNode *CloseParen();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Function_Context* function_();

  class  DxImageTransformContext : public antlr4::ParserRuleContext {
  public:
    DxImageTransformContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DxImageTransform();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    ExprContext *expr();
    antlr4::tree::TerminalNode *CloseParen();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DxImageTransformContext* dxImageTransform();

  class  HexcolorContext : public antlr4::ParserRuleContext {
  public:
    HexcolorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Hash();
    WsContext *ws();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  HexcolorContext* hexcolor();

  class  NumberContext : public antlr4::ParserRuleContext {
  public:
    NumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Number();
    antlr4::tree::TerminalNode *Plus();
    antlr4::tree::TerminalNode *Minus();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NumberContext* number();

  class  PercentageContext : public antlr4::ParserRuleContext {
  public:
    PercentageContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Percentage();
    antlr4::tree::TerminalNode *Plus();
    antlr4::tree::TerminalNode *Minus();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PercentageContext* percentage();

  class  DimensionContext : public antlr4::ParserRuleContext {
  public:
    DimensionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Dimension();
    antlr4::tree::TerminalNode *Plus();
    antlr4::tree::TerminalNode *Minus();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DimensionContext* dimension();

  class  UnknownDimensionContext : public antlr4::ParserRuleContext {
  public:
    UnknownDimensionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UnknownDimension();
    antlr4::tree::TerminalNode *Plus();
    antlr4::tree::TerminalNode *Minus();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnknownDimensionContext* unknownDimension();

  class  Any_Context : public antlr4::ParserRuleContext {
  public:
    Any_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    NumberContext *number();
    PercentageContext *percentage();
    DimensionContext *dimension();
    UnknownDimensionContext *unknownDimension();
    antlr4::tree::TerminalNode *String_();
    UrlContext *url();
    antlr4::tree::TerminalNode *Hash();
    antlr4::tree::TerminalNode *UnicodeRange();
    antlr4::tree::TerminalNode *Includes();
    antlr4::tree::TerminalNode *DashMatch();
    antlr4::tree::TerminalNode *Colon();
    antlr4::tree::TerminalNode *Function_();
    antlr4::tree::TerminalNode *CloseParen();
    std::vector<Any_Context *> any_();
    Any_Context* any_(size_t i);
    std::vector<UnusedContext *> unused();
    UnusedContext* unused(size_t i);
    antlr4::tree::TerminalNode *OpenParen();
    antlr4::tree::TerminalNode *OpenBracket();
    antlr4::tree::TerminalNode *CloseBracket();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Any_Context* any_();

  class  AtRuleContext : public antlr4::ParserRuleContext {
  public:
    AtRuleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    AtRuleContext() = default;
    void copyFrom(AtRuleContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  UnknownAtRuleContext : public AtRuleContext {
  public:
    UnknownAtRuleContext(AtRuleContext *ctx);

    antlr4::tree::TerminalNode *AtKeyword();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    BlockContext *block();
    antlr4::tree::TerminalNode *SemiColon();
    std::vector<Any_Context *> any_();
    Any_Context* any_(size_t i);
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  AtRuleContext* atRule();

  class  UnusedContext : public antlr4::ParserRuleContext {
  public:
    UnusedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BlockContext *block();
    antlr4::tree::TerminalNode *AtKeyword();
    WsContext *ws();
    antlr4::tree::TerminalNode *SemiColon();
    antlr4::tree::TerminalNode *Cdo();
    antlr4::tree::TerminalNode *Cdc();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnusedContext* unused();

  class  BlockContext : public antlr4::ParserRuleContext {
  public:
    BlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OpenBrace();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *CloseBrace();
    std::vector<DeclarationListContext *> declarationList();
    DeclarationListContext* declarationList(size_t i);
    std::vector<NestedStatementContext *> nestedStatement();
    NestedStatementContext* nestedStatement(size_t i);
    std::vector<Any_Context *> any_();
    Any_Context* any_(size_t i);
    std::vector<BlockContext *> block();
    BlockContext* block(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AtKeyword();
    antlr4::tree::TerminalNode* AtKeyword(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SemiColon();
    antlr4::tree::TerminalNode* SemiColon(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlockContext* block();

  class  NestedStatementContext : public antlr4::ParserRuleContext {
  public:
    NestedStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RulesetContext *ruleset();
    MediaContext *media();
    PageContext *page();
    FontFaceRuleContext *fontFaceRule();
    KeyframesRuleContext *keyframesRule();
    SupportsRuleContext *supportsRule();
    ViewportContext *viewport();
    CounterStyleContext *counterStyle();
    FontFeatureValuesRuleContext *fontFeatureValuesRule();
    AtRuleContext *atRule();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NestedStatementContext* nestedStatement();

  class  GroupRuleBodyContext : public antlr4::ParserRuleContext {
  public:
    GroupRuleBodyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OpenBrace();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *CloseBrace();
    std::vector<NestedStatementContext *> nestedStatement();
    NestedStatementContext* nestedStatement(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupRuleBodyContext* groupRuleBody();

  class  SupportsRuleContext : public antlr4::ParserRuleContext {
  public:
    SupportsRuleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Supports();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    SupportsConditionContext *supportsCondition();
    GroupRuleBodyContext *groupRuleBody();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SupportsRuleContext* supportsRule();

  class  SupportsConditionContext : public antlr4::ParserRuleContext {
  public:
    SupportsConditionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SupportsNegationContext *supportsNegation();
    SupportsConjunctionContext *supportsConjunction();
    SupportsDisjunctionContext *supportsDisjunction();
    SupportsConditionInParensContext *supportsConditionInParens();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SupportsConditionContext* supportsCondition();

  class  SupportsConditionInParensContext : public antlr4::ParserRuleContext {
  public:
    SupportsConditionInParensContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OpenParen();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    SupportsConditionContext *supportsCondition();
    antlr4::tree::TerminalNode *CloseParen();
    SupportsDeclarationConditionContext *supportsDeclarationCondition();
    GeneralEnclosedContext *generalEnclosed();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SupportsConditionInParensContext* supportsConditionInParens();

  class  SupportsNegationContext : public antlr4::ParserRuleContext {
  public:
    SupportsNegationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Not();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *Space();
    SupportsConditionInParensContext *supportsConditionInParens();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SupportsNegationContext* supportsNegation();

  class  SupportsConjunctionContext : public antlr4::ParserRuleContext {
  public:
    SupportsConjunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SupportsConditionInParensContext *> supportsConditionInParens();
    SupportsConditionInParensContext* supportsConditionInParens(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Space();
    antlr4::tree::TerminalNode* Space(size_t i);
    std::vector<antlr4::tree::TerminalNode *> And();
    antlr4::tree::TerminalNode* And(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SupportsConjunctionContext* supportsConjunction();

  class  SupportsDisjunctionContext : public antlr4::ParserRuleContext {
  public:
    SupportsDisjunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SupportsConditionInParensContext *> supportsConditionInParens();
    SupportsConditionInParensContext* supportsConditionInParens(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Space();
    antlr4::tree::TerminalNode* Space(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Or();
    antlr4::tree::TerminalNode* Or(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SupportsDisjunctionContext* supportsDisjunction();

  class  SupportsDeclarationConditionContext : public antlr4::ParserRuleContext {
  public:
    SupportsDeclarationConditionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OpenParen();
    WsContext *ws();
    DeclarationContext *declaration();
    antlr4::tree::TerminalNode *CloseParen();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SupportsDeclarationConditionContext* supportsDeclarationCondition();

  class  GeneralEnclosedContext : public antlr4::ParserRuleContext {
  public:
    GeneralEnclosedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CloseParen();
    antlr4::tree::TerminalNode *Function_();
    antlr4::tree::TerminalNode *OpenParen();
    std::vector<Any_Context *> any_();
    Any_Context* any_(size_t i);
    std::vector<UnusedContext *> unused();
    UnusedContext* unused(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GeneralEnclosedContext* generalEnclosed();

  class  UrlContext : public antlr4::ParserRuleContext {
  public:
    UrlContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Url_();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *String_();
    antlr4::tree::TerminalNode *CloseParen();
    antlr4::tree::TerminalNode *Url();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UrlContext* url();

  class  Var_Context : public antlr4::ParserRuleContext {
  public:
    Var_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Var();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *Variable();
    antlr4::tree::TerminalNode *CloseParen();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Var_Context* var_();

  class  CalcContext : public antlr4::ParserRuleContext {
  public:
    CalcContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Calc();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    CalcExprContext *calcExpr();
    antlr4::tree::TerminalNode *CloseParen();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CalcContext* calc();

  class  CalcOperandContext : public antlr4::ParserRuleContext {
  public:
    CalcOperandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CalcValueContext *calcValue();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CalcOperandContext* calcOperand();

  class  CalcSignContext : public antlr4::ParserRuleContext {
  public:
    CalcSignContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Plus();
    antlr4::tree::TerminalNode *Minus();
    antlr4::tree::TerminalNode *Divide();
    antlr4::tree::TerminalNode *Multiply();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CalcSignContext* calcSign();

  class  CalcExprContext : public antlr4::ParserRuleContext {
  public:
    CalcExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<CalcOperandContext *> calcOperand();
    CalcOperandContext* calcOperand(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Space();
    antlr4::tree::TerminalNode* Space(size_t i);
    std::vector<CalcSignContext *> calcSign();
    CalcSignContext* calcSign(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CalcExprContext* calcExpr();

  class  CalcNestedValueContext : public antlr4::ParserRuleContext {
  public:
    CalcNestedValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OpenParen();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    CalcExprContext *calcExpr();
    antlr4::tree::TerminalNode *CloseParen();
    antlr4::tree::TerminalNode *Minus();
    antlr4::tree::TerminalNode *Plus();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CalcNestedValueContext* calcNestedValue();

  class  CalcValueContext : public antlr4::ParserRuleContext {
  public:
    CalcValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumberContext *number();
    WsContext *ws();
    DimensionContext *dimension();
    UnknownDimensionContext *unknownDimension();
    PercentageContext *percentage();
    CalcNestedValueContext *calcNestedValue();
    CalcContext *calc();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CalcValueContext* calcValue();

  class  FontFaceRuleContext : public antlr4::ParserRuleContext {
  public:
    FontFaceRuleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FontFace();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *OpenBrace();
    antlr4::tree::TerminalNode *CloseBrace();
    std::vector<FontFaceDeclarationContext *> fontFaceDeclaration();
    FontFaceDeclarationContext* fontFaceDeclaration(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SemiColon();
    antlr4::tree::TerminalNode* SemiColon(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FontFaceRuleContext* fontFaceRule();

  class  FontFaceDeclarationContext : public antlr4::ParserRuleContext {
  public:
    FontFaceDeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    FontFaceDeclarationContext() = default;
    void copyFrom(FontFaceDeclarationContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  KnownFontFaceDeclarationContext : public FontFaceDeclarationContext {
  public:
    KnownFontFaceDeclarationContext(FontFaceDeclarationContext *ctx);

    Property_Context *property_();
    antlr4::tree::TerminalNode *Colon();
    WsContext *ws();
    ExprContext *expr();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  UnknownFontFaceDeclarationContext : public FontFaceDeclarationContext {
  public:
    UnknownFontFaceDeclarationContext(FontFaceDeclarationContext *ctx);

    Property_Context *property_();
    antlr4::tree::TerminalNode *Colon();
    WsContext *ws();
    ValueContext *value();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  FontFaceDeclarationContext* fontFaceDeclaration();

  class  KeyframesRuleContext : public antlr4::ParserRuleContext {
  public:
    KeyframesRuleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Keyframes();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *Space();
    IdentContext *ident();
    antlr4::tree::TerminalNode *OpenBrace();
    antlr4::tree::TerminalNode *CloseBrace();
    std::vector<KeyframeBlockContext *> keyframeBlock();
    KeyframeBlockContext* keyframeBlock(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  KeyframesRuleContext* keyframesRule();

  class  KeyframeBlockContext : public antlr4::ParserRuleContext {
  public:
    KeyframeBlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    KeyframeSelectorContext *keyframeSelector();
    antlr4::tree::TerminalNode *OpenBrace();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *CloseBrace();
    DeclarationListContext *declarationList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  KeyframeBlockContext* keyframeBlock();

  class  KeyframeSelectorContext : public antlr4::ParserRuleContext {
  public:
    KeyframeSelectorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    std::vector<antlr4::tree::TerminalNode *> From();
    antlr4::tree::TerminalNode* From(size_t i);
    std::vector<antlr4::tree::TerminalNode *> To();
    antlr4::tree::TerminalNode* To(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Percentage();
    antlr4::tree::TerminalNode* Percentage(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  KeyframeSelectorContext* keyframeSelector();

  class  ViewportContext : public antlr4::ParserRuleContext {
  public:
    ViewportContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Viewport();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *OpenBrace();
    antlr4::tree::TerminalNode *CloseBrace();
    DeclarationListContext *declarationList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ViewportContext* viewport();

  class  CounterStyleContext : public antlr4::ParserRuleContext {
  public:
    CounterStyleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CounterStyle();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    IdentContext *ident();
    antlr4::tree::TerminalNode *OpenBrace();
    antlr4::tree::TerminalNode *CloseBrace();
    DeclarationListContext *declarationList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CounterStyleContext* counterStyle();

  class  FontFeatureValuesRuleContext : public antlr4::ParserRuleContext {
  public:
    FontFeatureValuesRuleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FontFeatureValues();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    FontFamilyNameListContext *fontFamilyNameList();
    antlr4::tree::TerminalNode *OpenBrace();
    antlr4::tree::TerminalNode *CloseBrace();
    std::vector<FeatureValueBlockContext *> featureValueBlock();
    FeatureValueBlockContext* featureValueBlock(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FontFeatureValuesRuleContext* fontFeatureValuesRule();

  class  FontFamilyNameListContext : public antlr4::ParserRuleContext {
  public:
    FontFamilyNameListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<FontFamilyNameContext *> fontFamilyName();
    FontFamilyNameContext* fontFamilyName(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FontFamilyNameListContext* fontFamilyNameList();

  class  FontFamilyNameContext : public antlr4::ParserRuleContext {
  public:
    FontFamilyNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *String_();
    std::vector<IdentContext *> ident();
    IdentContext* ident(size_t i);
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FontFamilyNameContext* fontFamilyName();

  class  FeatureValueBlockContext : public antlr4::ParserRuleContext {
  public:
    FeatureValueBlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FeatureTypeContext *featureType();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *OpenBrace();
    antlr4::tree::TerminalNode *CloseBrace();
    std::vector<FeatureValueDefinitionContext *> featureValueDefinition();
    FeatureValueDefinitionContext* featureValueDefinition(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SemiColon();
    antlr4::tree::TerminalNode* SemiColon(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FeatureValueBlockContext* featureValueBlock();

  class  FeatureTypeContext : public antlr4::ParserRuleContext {
  public:
    FeatureTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AtKeyword();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FeatureTypeContext* featureType();

  class  FeatureValueDefinitionContext : public antlr4::ParserRuleContext {
  public:
    FeatureValueDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentContext *ident();
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    antlr4::tree::TerminalNode *Colon();
    std::vector<NumberContext *> number();
    NumberContext* number(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FeatureValueDefinitionContext* featureValueDefinition();

  class  IdentContext : public antlr4::ParserRuleContext {
  public:
    IdentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Ident();
    antlr4::tree::TerminalNode *MediaOnly();
    antlr4::tree::TerminalNode *Not();
    antlr4::tree::TerminalNode *And();
    antlr4::tree::TerminalNode *Or();
    antlr4::tree::TerminalNode *From();
    antlr4::tree::TerminalNode *To();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IdentContext* ident();

  class  WsContext : public antlr4::ParserRuleContext {
  public:
    WsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> Comment();
    antlr4::tree::TerminalNode* Comment(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Space();
    antlr4::tree::TerminalNode* Space(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WsContext* ws();

  class  ColorContext : public antlr4::ParserRuleContext {
  public:
    ColorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<WsContext *> ws();
    WsContext* ws(size_t i);
    std::vector<Color_componentContext *> color_component();
    Color_componentContext* color_component(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);
    antlr4::tree::TerminalNode *CloseParen();
    antlr4::tree::TerminalNode *Rgb();
    antlr4::tree::TerminalNode *Rgba();
    Color_alphaContext *color_alpha();
    antlr4::tree::TerminalNode *Divide();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColorContext* color();

  class  Color_alphaContext : public antlr4::ParserRuleContext {
  public:
    Color_alphaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CalcValueContext *calcValue();
    CalcContext *calc();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Color_alphaContext* color_alpha();

  class  Color_componentContext : public antlr4::ParserRuleContext {
  public:
    Color_componentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CalcValueContext *calcValue();
    CalcContext *calc();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Color_componentContext* color_component();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

