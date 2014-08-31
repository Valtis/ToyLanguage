#pragma once
#include "AstNode.h"

class VariableReadNode : public AstNode
{
public:
  VariableReadNode(int declaration_line, int variable_id) : AstNode(declaration_line), m_variable_id(variable_id)
  {

  }
  
  int VariableId() 
  { 
    return m_variable_id;  
  }

  void accept(AstVisitor *visitor) override
  {
    visitor->Visit(this);
  }
private:
  int m_variable_id;
};