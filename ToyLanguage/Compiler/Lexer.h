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

  void RemoveComment(std::string & line);
  
  std::pair<std::string, std::string> MathReservedWordFromString(const std::string &characters);
  std::pair<std::string, std::string> MatchNumberFromString(const std::string &characters);
  std::pair<std::string, std::string> MatchIdentifierFromString(const std::string &characters);
  std::pair<std::string, std::string> MatchRegexFromString(const std::string &characters, const std::regex &ex);

  std::unordered_map<std::string, TokenType> m_string_tokens;
  std::istream &m_input;
  std::vector<std::string> m_lines;
  std::vector<Token> m_syntax_tokens;
  std::vector<std::string> m_reserved_words;
  int m_current_line;

};

