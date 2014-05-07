#pragma once
#include <vector>
#include <unordered_map>
#include "Token.h"
class Parser
{
public:
  Parser(std::pair<std::vector<std::string>, std::vector<Token>> &tokens);
  ~Parser();

  void Parse();
private:;

  bool HasTokens();
  void NextToken();
  Token PeekNextToken();
  std::string GetTokenErrorInfo();

  void ParseFunction();
  void ParseVariableList();
  void ParseBlock();

  void Expect(TokenType type);
  void ExpectType();
  void ExpectNonVoidType();

  void InvalidTokenError(std::string expected);


  std::vector<std::string> m_lines;
  std::vector<Token> m_tokens;
  std::vector<Token>::iterator m_current_token;

  std::unordered_map<TokenType, std::string> token_to_string;
};

