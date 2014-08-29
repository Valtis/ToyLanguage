#pragma once
#include "AstNode.h"

class RootNode : public AstNode
{
public:
  RootNode(int declaration_line) : AstNode(declaration_line)
  {

  }

  void accept(AstVisitor *visitor) override
  {
    visitor->visit(this);
  }
};