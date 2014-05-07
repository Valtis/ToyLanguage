#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "OperationType.h"
#include "Variable.h"
class AbstractSyntaxTreeNode;

typedef std::shared_ptr < AbstractSyntaxTreeNode > Ast_Node;

class AbstractSyntaxTreeNode
{
public:
  AbstractSyntaxTreeNode();
  AbstractSyntaxTreeNode(OperationType type);


  ~AbstractSyntaxTreeNode();

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

  AbstractSyntaxTreeNode *Parent()
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

  bool VariableExists(std::string name);

  void AddVariable(std::string name, Variable variable);
  Variable GetVariable(std::string name);
  
  void SetValue(Variable value) { m_value = value;  }
  Variable Value() { return m_value;  }

private:
  OperationType m_type;
  Variable m_value; // ignored unless operation_type is value; value for root node. Used as leaf value when constructing byte code from ast
  
  AbstractSyntaxTreeNode *m_parent;
  std::vector<Ast_Node> m_children;
  std::unordered_map<std::string, Variable> m_visible_variables;
};

