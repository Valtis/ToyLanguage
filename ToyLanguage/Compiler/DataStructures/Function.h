#pragma once
#include <string>
#include <memory>
#include "AstNode.h"
class Function
{
public:
  Function();
  Function(std::string name, int declaration_line);
  ~Function();

  void SetRootNode(std::shared_ptr<AstNode> node);
  std::shared_ptr<AstNode> RootNode() { return m_root_node; }
  std::string Name() const { return m_name;  }

  int DeclarationLine() const { return m_declaration_line;  }


  
private:
  int m_declaration_line;
  std::string m_name;
  Ast_Node m_root_node;

};

