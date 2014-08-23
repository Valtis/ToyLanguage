#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include "../DataStructures/Function.h"
#include "../DataStructures/Token.h"


class Parser
{
public:
  Parser(std::pair<std::vector<std::string>, std::vector<Token>> &tokens);
  ~Parser();

  std::unordered_map<std::string, Function>  Parse();
private:

  bool HasTokens();
  Token CurrentToken();
  Token NextToken();
  Token PeekToken();
  
  Token Expect(TokenType type);
  void ExpectType();
  void ExpectNonVoidType();

  void ParseFunctionDeclaration();
  void ParseFunctionArguments(Function &f);
  void ParseFunctionBody(Function &f);
  std::vector<std::string> m_lines;
  std::vector<Token> m_tokens;
  std::vector<Token>::iterator m_current_token;
  std::unordered_map<TokenType, std::string> token_to_string;
  std::unordered_map<std::string, Function> m_functions;

  Ast_Node m_root_node;

};

