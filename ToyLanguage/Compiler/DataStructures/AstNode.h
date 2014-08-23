#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "OperationType.h"
#include "Variable.h"
class AstNode;

typedef std::shared_ptr < AstNode > Ast_Node;

class AstNode
{
public:
  AstNode();
  AstNode(OperationType type);


  ~AstNode();
    
  void AddChild(Ast_Node node)
  {
    node->m_parent = this;
    m_children.push_back(std::move(node));
  }

  void RemoveChild(Ast_Node node)
  {
    for (auto iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
      if ((*iter) == node)
      {
        m_children.erase(iter);
        return;
      }
    }
    
  }

  AstNode *Parent()
  {
    return m_parent;
  }

  std::vector<Ast_Node> Children()
  {
    return m_children;
  }

  void SetType(OperationType type)
  {
    m_type = type;
  }

  OperationType Type()
  {
    return m_type;
  }

private:
  OperationType m_type;

  AstNode *m_parent;
  std::vector<Ast_Node> m_children;

};

