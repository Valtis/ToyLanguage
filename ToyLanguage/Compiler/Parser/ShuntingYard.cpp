#include "ShuntingYard.h"
#include "ShuntingYardError.h"
#include <algorithm>
ShuntingYard::ShuntingYard(std::unordered_map<std::string, Function> declared_functions) : m_declared_functions(declared_functions)
{
  // TODO: clean up
  m_priorities[TokenType::MULTIPLICATION] = 2;
  m_priorities[TokenType::DIVISION] = 2;
  m_priorities[TokenType::PLUS] = 1;
  m_priorities[TokenType::MINUS] = 1;
  m_priorities[TokenType::ASSIGNMENT] = 0;

  m_token_to_operation[TokenType::MULTIPLICATION] = OperationType::MUL;
  m_token_to_operation[TokenType::DIVISION] = OperationType::DIV;
  m_token_to_operation[TokenType::PLUS] = OperationType::ADD;
  m_token_to_operation[TokenType::MINUS] = OperationType::SUB;
  m_token_to_operation[TokenType::ASSIGNMENT] = OperationType::ASSIGNMENT;
  
  m_token_associativeness[TokenType::MULTIPLICATION] = Associativeness::LEFT;
  m_token_associativeness[TokenType::DIVISION] = Associativeness::LEFT;
  m_token_associativeness[TokenType::PLUS] = Associativeness::LEFT;
  m_token_associativeness[TokenType::MINUS] = Associativeness::LEFT;
  m_token_associativeness[TokenType::ASSIGNMENT] = Associativeness::RIGHT;
}


ShuntingYard::~ShuntingYard()
{
}



Ast_Node ShuntingYard::CreateAst(std::vector<Token> tokens, Ast_Node parent_node)
{
  ToReversePolish(tokens);

  auto root = CreateAstFromReversePolish(parent_node);

 // ValidateTree(root);  // TODO: checks that generated tree is valid (valid variable types for operators, no operators as leaf nodes etc)
  return root;
}


void ShuntingYard::ToReversePolish(std::vector<Token> tokens)
{
  for (auto token : tokens)
  {
    if (token.Type() == TokenType::IDENT || token.Type() == TokenType::NUMBER)
    {
      if (IsFunction(token))
      {
        m_operation_stack.push_back(token);
      }
      else 
      {
        m_output_queue.push_back(token);
      }
    }
    else if (token.Type() == TokenType::LPAREN)
    {
      m_operation_stack.push_back(token);
    }
    else if (token.Type() == TokenType::RPAREN)
    {
      HandleClosingParenthesis(token);
    }
    else if (m_priorities.count(token.Type()) != 0)
    {
      if (m_operation_stack.size() > 0 && m_priorities[token.Type()] <= m_priorities[m_operation_stack.back().Type()])
      {
        AddHigherPrecedenceOperatorsToOutputQueue(token);
      }
      m_operation_stack.push_back(token);
    }
  }

  PopStackToQueue();
}

void ShuntingYard::HandleClosingParenthesis( Token token )
{
  while (m_operation_stack.size() > 0 && m_operation_stack.back().Type() != TokenType::LPAREN)
  {
    m_output_queue.push_back(m_operation_stack.back());
    m_operation_stack.pop_back();
  }

  if (m_operation_stack.size() == 0)
  {
    throw ShuntingYardMissingParenthesisError(token);
  }

  m_operation_stack.pop_back();
  
}

void ShuntingYard::AddHigherPrecedenceOperatorsToOutputQueue( Token token )
{
  int token_priority = m_priorities[token.Type()];

  while (m_operation_stack.size() > 0 && ((m_priorities[m_operation_stack.back().Type()] > token_priority) || 
    (m_token_associativeness[token.Type()] == Associativeness::LEFT && m_priorities[m_operation_stack.back().Type()] == token_priority )))
  {
    m_output_queue.push_back(m_operation_stack.back());
    m_operation_stack.pop_back();
  }
}

void ShuntingYard::PopStackToQueue()
{
  while (m_operation_stack.size() > 0)
  {
    auto token = m_operation_stack.back();
    if (token.Type() == TokenType::LPAREN)
    {
      throw ShuntingYardMissingParenthesisError(token);
    }
    m_output_queue.push_back(token);
    m_operation_stack.pop_back();
  }
}

Ast_Node ShuntingYard::CreateAstFromReversePolish(Ast_Node parent_node)
{
  std::vector<Ast_Node> astNodes;

  while (m_output_queue.size() > 0)
  {
    auto type = m_output_queue.front().Type();
    if (type == TokenType::NUMBER)
    {
      AddNumberNode(astNodes);
    }
    else if (type == TokenType::IDENT)
    {
      if (IsFunction(m_output_queue.front()))
      {
        AddFunctionCall(astNodes);
      }
      else
      {
        AddVariableNode(astNodes, parent_node);
      }
    }
    else
    {
      AddOperatorNode(astNodes, m_output_queue.front());
    }
    m_output_queue.pop_front();
  }

  if (astNodes.size() > 1)
  {
    throw std::logic_error("AST generation error - unused AST token when parsing expression");
  }
  else if (astNodes.size() == 0)
  {
    throw std::logic_error("AST generation error - no AST tokens generated when parsing expression");
  }
  return astNodes[0];
}

Variable ShuntingYard::GetVariable()
{
  Variable v{ "", m_output_queue.front().LineNumber() };
  // assumed int for now
  v.SetValue(VariableType::INTEGER, std::stoi(m_output_queue.front().Value()));
  return v;
}

void ShuntingYard::AddNumberNode(std::vector<Ast_Node> &astNodes)
{
  Ast_Node node{ new AbstractSyntaxTreeNode{ OperationType::CONSTANT }};
  node->SetValue(GetVariable());
  astNodes.push_back(node);
}


void ShuntingYard::AddVariableNode(std::vector<Ast_Node> &astNodes, Ast_Node parent_node)
{
  if (!parent_node->VariableExists(m_output_queue.front().Value()))
  {
    throw ShuntingYardUndeclaredVariableError(m_output_queue.front());
  }

  Ast_Node node{ new AbstractSyntaxTreeNode{ OperationType::VARIABLE } };
  node->SetValue(parent_node->GetVariable(m_output_queue.front().Value()));
  astNodes.push_back(node);
}

void ShuntingYard::AddFunctionCall(std::vector<Ast_Node> &astNodes)
{
  Ast_Node node{ new AbstractSyntaxTreeNode{ OperationType::FUNCTION_CALL } };
  auto token = m_output_queue.front();
  
  Variable v(token.Value(), token.LineNumber());

  node->SetValue(v);

  astNodes.push_back(node);
}


void ShuntingYard::AddOperatorNode(std::vector<Ast_Node> &astNodes, Token token)
{
  Ast_Node node;
  if (astNodes.size() < 2)
  {
    throw ShuntingYardMissingOperandError(token);
  }

  node = Ast_Node{ new AbstractSyntaxTreeNode{ m_token_to_operation[token.Type()] } };

  auto right = astNodes.back();
  astNodes.pop_back();
  auto left = astNodes.back();
  astNodes.pop_back();

  node->AddChild(left);
  node->AddChild(right);
  astNodes.push_back(node);
}


bool ShuntingYard::IsFunction(Token token)
{
  return m_declared_functions.count(token.Value()) != 0;
}

