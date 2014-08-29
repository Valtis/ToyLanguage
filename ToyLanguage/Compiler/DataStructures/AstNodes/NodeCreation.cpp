#include "NodeCreation.h"
#include "RootNode.h"
#include "NumberNode.h"
#include "VariableReadNode.h"
#include "FunctionCallNode.h"

Ast_Node CreateRootNode(int declaration_line)
{
  return std::make_shared<RootNode>(declaration_line);
}

Ast_Node CreateNumberNode(int declaration_line, double value)
{
  return std::make_shared<NumberNode>(declaration_line, value);
}

Ast_Node CreateVariableReadNode(int declaration_line, int variable_id)
{
  return std::make_shared<VariableReadNode>(declaration_line, variable_id);
}

Ast_Node CreateFunctionCallNode(int declaration_line, const std::string &name)
{
  return std::make_shared<FunctionCallNode>(declaration_line, name);
}