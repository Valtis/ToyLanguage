#pragma once
#include "../Compiler/DataStructures/AstNodes/AstNode.h"
#include "../Compiler/DataStructures/AstNodes/AstVisitor.h"
void TraverseAst(Ast_Node &node, AstVisitor *visitor);