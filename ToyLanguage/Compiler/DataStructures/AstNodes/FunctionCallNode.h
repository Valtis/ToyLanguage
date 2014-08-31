#pragma once
#include <string>
#include "AstNode.h"

class FunctionCallNode : public AstNode
{
public:
  FunctionCallNode(int declaration_line, const std::string &name) : AstNode(declaration_line), m_function_name(name)
  {

  }


  std::string Name()
  {
    return m_function_name;
  }

  void accept(AstVisitor *visitor) override
  {
    visitor->Visit(this);
  }

private:

  std::string m_function_name;
};