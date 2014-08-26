#pragma once
#include <functional>
#include "../Compiler/DataStructures/AstNode.h"

void TransformAst(Ast_Node &node, const std::function<void(Ast_Node &node)> &transformer);