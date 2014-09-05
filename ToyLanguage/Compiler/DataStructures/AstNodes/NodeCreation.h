#pragma once
#include "AstNode.h"

AstPtr CreateRootNode(int declaration_line);
AstPtr CreateNumberNode(int declaration_line, double value);
AstPtr CreateVariableReadNode(int declaration_line, int variable_id);
AstPtr CreateFunctionCallNode(int declaration_line, const std::string &name);
AstPtr CreateFunctionParameterNode(int declaration_line, const std::string &name);

AstPtr CreateIfBooleanNode();
AstPtr CreateIfThenNode();