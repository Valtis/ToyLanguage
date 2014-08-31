#pragma once
#include <string>
#include "AstNode.h"

class IfThenNode : public AstNode
{
public:
  IfThenNode() : AstNode(0)
  {

  }

  void accept(AstVisitor *visitor) override
  {
    visitor->Visit(this);
  }

private:

};