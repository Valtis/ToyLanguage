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
  std::string Name() const { return m_name;  }
  int DeclarationLine() const { return m_declaration_line;  }
  int ParameterCount() const { return m_parameters.size();  }

private:
  int m_declaration_line;
  std::string m_name;
  Ast_Node m_root_node;
  std::vector<VariableType> m_parameters;
};

