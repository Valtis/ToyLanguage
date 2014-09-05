#include "NodeCreation.h"
#include "RootNode.h"
#include "NumberNode.h"
#include "VariableReadNode.h"
#include "FunctionCallNode.h"
#include "FunctionParameterNode.h"

#include "IfBooleanNode.h"
#include "IfThenNode.h"


AstPtr CreateRootNode(int declaration_line)
{
  return std::make_shared<RootNode>(declaration_line);
}

AstPtr CreateNumberNode(int declaration_line, double value)
{
  return std::make_shared<NumberNode>(declaration_line, value);
}

AstPtr CreateVariableReadNode(int declaration_line, int variable_id)
{
  return std::make_shared<VariableReadNode>(declaration_line, variable_id);
}

AstPtr CreateFunctionCallNode(int declaration_line, const std::string &name)
{
  return std::make_shared<FunctionCallNode>(declaration_line, name);
}

AstPtr CreateFunctionParameterNode(int declaration_line, const std::string &name)
{
  return std::make_shared<FunctionParameterNode>(declaration_line, name);
}




AstPtr CreateIfBooleanNode()
{
  return std::make_shared<IfBooleanNode>();
}

AstPtr CreateIfThenNode()
{
  return std::make_shared<IfThenNode>();
}