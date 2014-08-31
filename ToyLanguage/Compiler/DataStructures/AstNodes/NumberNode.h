#pragma once

#include "AstNode.h"

class NumberNode : public AstNode
{
public:
  NumberNode(int declaration_line, double value) : AstNode(declaration_line), m_value(value)
  {

  }

  double Value()
  {
    return m_value;
  }

  void accept(AstVisitor *visitor) override
  {
    visitor->Visit(this);
  }

private:
  double m_value;

};