#pragma once

class NumberNode;
class VariableReadNode;
class RootNode;
class FunctionCallNode;
class FunctionParameterNode;

class IfStartNode;
class IfBooleanNode;
class IfThenNode;

class AstVisitor
{
public:

  virtual void Visit(RootNode *node)
  {

  }
  
  virtual void Visit(NumberNode *node)
  {

  }
  virtual void Visit(VariableReadNode *node)
  {

  }

  virtual void Visit(FunctionCallNode *node)
  {

  }
  
  virtual void Visit(IfBooleanNode *node)
  {

  }
  
  virtual void Visit(IfThenNode *node)
  {

  }

  virtual void Visit(FunctionParameterNode *node)
  {

  }
};