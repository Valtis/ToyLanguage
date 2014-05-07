#pragma once
#include <vector>
#include <unordered_map>
#include "Function.h"
#include "Token.h"


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
  std::string GetTokenErrorInfo();

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
  void AddBlockNode();
  void VariableRedeclarationError();
  void UndeclaredVariableError();
  void AssignmentHelper(Ast_Node &root, Ast_Node &node, Ast_Node child);
  void ExpectExpressionType();
  bool IsOperator(TokenType type);

  std::vector<std::string> m_lines;
  std::vector<Token> m_tokens;
  std::vector<Token>::iterator m_current_token;
  std::unordered_map<TokenType, std::string> token_to_string;
  std::unordered_map<OperationType, int> m_operation_expression_priority;

  std::unordered_map<std::string, Function> m_functions;

  Ast_Node m_root_node;

};

