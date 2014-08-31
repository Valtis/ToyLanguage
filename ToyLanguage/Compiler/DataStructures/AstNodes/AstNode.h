#pragma once
#include <deque>
#include <memory>

class AstNode;

typedef std::shared_ptr < AstNode > AstPtr;
#include "AstVisitor.h"


class AstNode
{
public:
  AstNode(int line);


  ~AstNode();
    
  void AddChild(AstPtr node);
  
  void AddChildToLeft(AstPtr node);

  void RemoveChild(AstPtr node);

  void ReplaceChild(AstPtr old_node, AstPtr new_node);

  AstNode *Parent() const;

  AstPtr GetChildByRawPointer(AstNode *node);

  std::deque<AstPtr> Children() const;

  int DeclarationLine() const;
  

  virtual void accept(AstVisitor *visitor) = 0;

private:

  AstNode *m_parent;
  std::deque<AstPtr> m_children;
  int m_declaration_line;

};

