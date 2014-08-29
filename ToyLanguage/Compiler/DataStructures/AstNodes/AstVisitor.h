#pragma once

class NumberNode;
class VariableReadNode;
class RootNode;
class FunctionCallNode;

class AstVisitor
{
public:

  virtual void visit(RootNode *node) = 0;
  virtual void visit(NumberNode *node) = 0;
  virtual void visit(VariableReadNode *node) = 0;
  virtual void visit(FunctionCallNode *node) = 0;

};