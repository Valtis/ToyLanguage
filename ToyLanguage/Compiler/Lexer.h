#pragma once
#include <istream>
#include <unordered_map>
#include <string>
#include <vector>
#include <regex>
#include "DataStructures/Token.h"


class Lexer
{
public:
  Lexer(std::istream &input);
  ~Lexer();

  std::pair<std::vector<std::string>, std::vector<Token>> AnalyzeText();
  
private:
  
  

  void HandleLines();

  void HandleLine(std::string &line);

  void HandleTokens(const std::string &str_token);

  void HandleComplexToken(const std::string &token);

  bool MatchSingleCharacterToken(const std::string &token);

  void RemoveComment(std::string & line);
  
  struct Match
  {
  public:
    std::string match_string;
    std::string remnant_string;
  };

  bool MatchIdentifier(const std::string & token);
  bool MatchTypeOf(TokenType type, Match match);

  bool MatchNumber(const std::string &token);
 
  Match MathReservedWordFromString(const std::string &characters);
  Match MatchNumberFromString(const std::string &characters);
  Match MatchIdentifierFromString(const std::string &characters);
  Match MatchRegexFromString(const std::string &characters, const std::regex &ex);
  bool MatchReservedWord(const std::string &token);

  std::unordered_map<std::string, TokenType> m_string_tokens;
  std::istream &m_input;
  std::vector<std::string> m_lines;
  std::vector<Token> m_syntax_tokens;
  std::vector<std::string> m_reserved_words;
  int m_current_line;

};

