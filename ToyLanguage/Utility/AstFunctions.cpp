#include "AstFunctions.h"

void TraverseAst(AstPtr &node, AstVisitor *visitor)
{
  if (node == nullptr)
  {
    return;
  }

  for (auto node : node->Children())
  {
    TraverseAst(node, visitor);
  }

  node->accept(visitor);
}