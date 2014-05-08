#include "ShuntingYard.h"
#include "../ParseError.h"

ShuntingYard::ShuntingYard()
{
  m_priorities[TokenType::MULTIPLICATION] = 1;
  m_priorities[TokenType::DIVISION] = 1;
  m_priorities[TokenType::PLUS] = 0;
  m_priorities[TokenType::MINUS] = 0;

  m_token_to_operation[TokenType::MULTIPLICATION] = OperationType::MUL;
  m_token_to_operation[TokenType::DIVISION] = OperationType::DIV;
  m_token_to_operation[TokenType::PLUS] = OperationType::ADD;
  m_token_to_operation[TokenType::MINUS] = OperationType::SUB;
}


ShuntingYard::~ShuntingYard()
{
}



Ast_Node ShuntingYard::CreateAst(std::vector<Token> tokens, Ast_Node parent_node)
{
  ToReversePolish(tokens);

  auto root = CreateAstFromReversePolish(parent_node);

 // ValidateTree(root);
  return root;
}


void ShuntingYard::ToReversePolish(std::vector<Token> tokens)
{
  for (auto token : tokens)
  {
    if (token.Type() == TokenType::IDENT || token.Type() == TokenType::NUMBER)
    {
      // TODO: check for function call
      m_output_queue.push_back(token);
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

void ShuntingYard::AddHigherPrecedenceOperatorsToOutputQueue( Token token )
{
  while (m_operation_stack.size() > 0 && m_priorities[m_operation_stack.back().Type()] >= m_priorities[token.Type()])
  {
    m_output_queue.push_back(m_operation_stack.back());
    m_operation_stack.pop_back();
  }
}

void ShuntingYard::PopStackToQueue()
{
  while (m_operation_stack.size() > 0)
  {
    m_output_queue.push_back(m_operation_stack.back());
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
      AddVariableNode(astNodes, parent_node);

    }
    else
    {
      AddOperatorNode(astNodes, type);
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
  Ast_Node node(new AbstractSyntaxTreeNode{ OperationType::CONSTANT });
  node->SetValue(GetVariable());
  astNodes.push_back(node);
}

void ShuntingYard::AddVariableNode(std::vector<Ast_Node> &astNodes, Ast_Node parent_node)
{
  if (!parent_node->VariableExists(m_output_queue.front().Value()))
  {
    throw UndeclaredVariableError(m_output_queue.front());
  }

  Ast_Node node(new AbstractSyntaxTreeNode{ OperationType::VARIABLE });
  node->SetValue(parent_node->GetVariable(m_output_queue.front().Value()));
  astNodes.push_back(node);
}

void ShuntingYard::AddOperatorNode(std::vector<Ast_Node> &astNodes, TokenType type)
{
  Ast_Node node;
  if (astNodes.size() < 2)
  {
    throw ParseError("Not enough tokens for expression parsing");
  }

  node = Ast_Node(new AbstractSyntaxTreeNode{ m_token_to_operation[type] });

  auto right = astNodes.back();
  astNodes.pop_back();
  auto left = astNodes.back();
  astNodes.pop_back();

  node->AddChild(left);
  node->AddChild(right);
  astNodes.push_back(node);
}

