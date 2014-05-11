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

  void HandleClosingParenthesis(Token token);

  void AddHigherPrecedenceOperatorsToOutputQueue(Token token);

  Ast_Node CreateAstFromReversePolish(Ast_Node parent_node);

  void AddOperatorNode(std::vector<Ast_Node> &astNodes, Token type);

  void AddVariableNode(std::vector<Ast_Node> &astNodes, Ast_Node parent_node);

  void AddNumberNode(std::vector<Ast_Node> &astNodes);

 

  Variable GetVariable();

  enum class Associativeness { LEFT, RIGHT };
  std::unordered_map<TokenType, int> m_priorities;
  std::unordered_map<TokenType, OperationType> m_token_to_operation;
  std::unordered_map<TokenType, Associativeness> m_token_associativeness;
  
  std::vector<Token> m_operation_stack;
  std::list<Token> m_output_queue;
};

