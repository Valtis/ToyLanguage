#include "AstFunctions.h"

void TransformAst(Ast_Node &node, const std::function<void(Ast_Node &node)> &transformer)
{
  if (node == nullptr)
  {
    return;
  }

  for (auto node : node->Children())
  {
    TransformAst(node, transformer);
  }

  transformer(node);
}