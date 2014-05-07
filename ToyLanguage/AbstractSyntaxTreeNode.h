#pragma once
#include <vector>
#include "OperationType.h"

class AbstractSyntaxTreeNode
{
public:
  AbstractSyntaxTreeNode();
  ~AbstractSyntaxTreeNode();

private:
  OperationType m_type;
  std::vector<AbstractSyntaxTreeNode> m_children;

};

