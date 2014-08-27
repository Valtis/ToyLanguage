#include "AstFunctions.h"

void TraverseAst(Ast_Node &node, const std::function<void(Ast_Node &node)> &transformer)
{
  if (node == nullptr)
  {
    return;
  }

  for (auto node : node->Children())
  {
    TraverseAst(node, transformer);
  }

  transformer(node);
}