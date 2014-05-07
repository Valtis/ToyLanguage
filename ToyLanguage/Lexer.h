#pragma once
#include <istream>
#include <unordered_map>
#include <string>
#include <vector>

#include "Token.h"


class Lexer
{
public:
  Lexer(std::istream &input);
  ~Lexer();

  std::vector<Token> AnalyzeText();
  
private:
  std::istream &m_input;
  std::vector<Token> m_syntax_tokens;

  void HandleLines();

  void HandleLine(std::string &line);

  void HandleTokens(std::string &str_token);

  void HandleComplexToken(std::string token);

  void AddTokenFromCharacters(std::string characters);

  std::unordered_map<std::string, TokenType> m_string_tokens;
};

