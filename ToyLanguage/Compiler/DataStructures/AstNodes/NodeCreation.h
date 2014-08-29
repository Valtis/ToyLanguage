#pragma once
#include "AstNode.h"

Ast_Node CreateRootNode(int declaration_line);
Ast_Node CreateNumberNode(int declaration_line, double value);
Ast_Node CreateVariableReadNode(int declaration_line, int variable_id);
Ast_Node CreateFunctionCallNode(int declaration_line, const std::string &name);