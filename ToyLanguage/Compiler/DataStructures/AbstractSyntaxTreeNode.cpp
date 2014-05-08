#include "AbstractSyntaxTreeNode.h"


AbstractSyntaxTreeNode::AbstractSyntaxTreeNode() : m_type(OperationType::BLOCK)
{
}

AbstractSyntaxTreeNode::AbstractSyntaxTreeNode(OperationType type) : m_type(type)
{
}

AbstractSyntaxTreeNode::~AbstractSyntaxTreeNode()
{
}

void AbstractSyntaxTreeNode::AddVariable(std::string name, Variable variable)
{
  if (m_type == OperationType::BLOCK)
  {
    m_visible_variables[name] = variable; 
  }


  if (m_parent != nullptr)
  {
    m_parent->AddVariable(name, variable);
  }
}

bool AbstractSyntaxTreeNode::VariableExists(std::string name)
{
  if (m_type == OperationType::BLOCK)
  {
    if (m_visible_variables.count(name) != 0)
    {
      return true;
    }
  }

  if (m_parent != nullptr)
  {
    return m_parent->VariableExists(name);
  }
  return false;
}


Variable AbstractSyntaxTreeNode::GetVariable(std::string name)
{
  if (m_type == OperationType::BLOCK && m_visible_variables.count(name) != 0)
  {
    return m_visible_variables[name];
  }

  if (m_parent != nullptr)
  {
    return m_parent->GetVariable(name);
  }

  return Variable{};
}