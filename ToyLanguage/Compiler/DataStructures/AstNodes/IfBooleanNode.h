#pragma once
#include <string>
#include "AstNode.h"

class IfBooleanNode : public AstNode
{
public:
  IfBooleanNode() : AstNode(0)
  {

  }

  void accept(AstVisitor *visitor) override
  {
    visitor->Visit(this);
  }

private:

};