#pragma once
#include <vector>
#include "Token.h"
class Parser
{
public:
  Parser(std::vector<Token> &tokens);
  ~Parser();

  void Parse();
private:;

  bool HasTokens();
  void NextToken();


  std::vector<Token> m_tokens;
  Token m_current_token;
};

