#pragma once
#include <vector>
#include <unordered_map>
#include "DataStructures/Function.h"
#include "DataStructures/Token.h"


class Parser
{
public:
  Parser(std::pair<std::vector<std::string>, std::vector<Token>> &tokens);
  ~Parser();

  std::unordered_map<std::string, Function>  Parse();
private:;

  bool HasTokens();
  void NextToken();
  Token PeekNextToken();
  

  void ParseFunction();
  void ParseVariableList();
  void ParseBlock(Ast_Node parent_node);
  void ParseStatement();

  void ParseVariableDeclaration(Ast_Node parent_node);
  void ParseExpression(Ast_Node parent_node);
  void ParseFunctionCallParameters();
  

  void Expect(TokenType type);
  void ExpectType();
  void ExpectNonVoidType();


  void InvalidTokenError(std::string expected);
  void FunctionRedeclarationError();
  void VariableRedeclarationError();
  void UndeclaredVariableError();
  std::string GetTokenErrorInfo();

  std::vector<std::string> m_lines;
  std::vector<Token> m_tokens;
  std::vector<Token>::iterator m_current_token;
  std::unordered_map<TokenType, std::string> token_to_string;
  std::unordered_map<std::string, Function> m_functions;

  Ast_Node m_root_node;

};

