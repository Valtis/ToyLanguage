#pragma once
#include "../DataStructures/Token.h"
#include "../DataStructures/AbstractSyntaxTreeNode.h"
class ShuntingYard
{
public:
  ShuntingYard();
  ~ShuntingYard();

  Ast_Node CreateAst(std::vector<Token> tokens, Ast_Node parent_node);
private:
  void ToReversePolish(std::vector<Token> tokens);

  void PopStackToQueue();

  void AddHigherPrecedenceOperatorsToOutputQueue(Token token);

  Ast_Node CreateAstFromReversePolish(Ast_Node parent_node);

  void AddOperatorNode(std::vector<Ast_Node> &astNodes, TokenType type);

  void AddVariableNode(std::vector<Ast_Node> &astNodes, Ast_Node parent_node);

  void AddNumberNode(std::vector<Ast_Node> &astNodes);

  Variable GetVariable();

  std::unordered_map<TokenType, int> m_priorities;
  std::unordered_map<TokenType, OperationType> m_token_to_operation;
  std::vector<Token> m_operation_stack;
  std::list<Token> m_output_queue;
};

