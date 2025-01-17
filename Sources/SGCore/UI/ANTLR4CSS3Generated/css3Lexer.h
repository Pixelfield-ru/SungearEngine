
// Generated from css3Lexer.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  css3Lexer : public antlr4::Lexer {
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
    ERROR = 2
  };

  explicit css3Lexer(antlr4::CharStream *input);

  ~css3Lexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

