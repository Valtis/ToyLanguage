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

  void HandleLines(std::vector<Token> &syntax_tokens);

  void HandleLine(std::string &line, std::vector<Token> &syntax_tokens);

  void HandleComplexToken(std::string token, std::vector<Token> &syntax_tokens);

  std::unordered_map<std::string, TokenType> m_string_tokens;
};

