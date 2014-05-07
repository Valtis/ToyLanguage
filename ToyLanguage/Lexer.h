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

  std::pair<std::vector<std::string>, std::vector<Token>> AnalyzeText();
  
private:
  
  void HandleLines();

  void HandleLine(std::string &line);

  void HandleTokens(std::string &str_token);

  void HandleComplexToken(std::string token);

  void AddTokenFromCharacters(std::string characters);
  void RemoveComment(std::string & line);
  std::unordered_map<std::string, TokenType> m_string_tokens;
  std::istream &m_input;
  std::vector<std::string> m_lines;
  std::vector<Token> m_syntax_tokens;
  int m_current_line;

};

