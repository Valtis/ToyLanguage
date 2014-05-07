#pragma once
#include <string>
#include <memory>
#include "AbstractSyntaxTreeNode.h"
class Function
{
public:
  Function();
  Function(std::string name, int declaration_line);
  ~Function();

  void SetRootNode(std::shared_ptr<AbstractSyntaxTreeNode> node);
  std::shared_ptr<AbstractSyntaxTreeNode> RootNode() { return m_root_node; }
  std::string Name() { return m_name;  }
  int DeclarationLine() { return m_declaration_line;  }

private:
  int m_declaration_line;
  std::string m_name;
  Ast_Node m_root_node;
};

