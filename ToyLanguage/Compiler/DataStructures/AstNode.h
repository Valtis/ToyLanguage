#pragma once
#include <vector>
#include <unordered_map>
#include <memory>

#include "Variable.h"
class AstNode;


enum class NodeType { ROOT, FUNCTION_CALL, NUMBER };

union NodeValue
{
  double number;
};

typedef std::shared_ptr < AstNode > Ast_Node;

class AstNode
{
public:
  AstNode(NodeType type);


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

  void SetType(NodeType type)
  {
    m_type = type;
  }

  NodeType Type()
  {
    return m_type;
  }

  void ValueAsNumber(const std::string &number)
  {
    m_value.number = std::stod(number);
  }

  double ValueAsNumber()
  {
    return m_value.number;
  }

  
private:
  NodeType m_type;
  AstNode *m_parent;
  std::vector<Ast_Node> m_children;

  NodeValue m_value;

};

