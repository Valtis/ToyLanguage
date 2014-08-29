#pragma once
#include <vector>
#include <memory>

class AstNode;

typedef std::shared_ptr < AstNode > Ast_Node;
#include "AstVisitor.h"


class AstNode
{
public:
  AstNode(int line);


  ~AstNode();
    
  void AddChild(Ast_Node node);

  void RemoveChild(Ast_Node node);

  void ReplaceChild(Ast_Node old_node, Ast_Node new_node);

  AstNode *Parent() const;

  Ast_Node GetChildByRawPointer(AstNode *node);

  std::vector<Ast_Node> Children() const;

  int DeclarationLine() const;
  

  virtual void accept(AstVisitor *visitor) = 0;

private:

  AstNode *m_parent;
  std::vector<Ast_Node> m_children;
  int m_declaration_line;

};

