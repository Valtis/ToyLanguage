#include "AstNode.h"

AstNode::AstNode(int line) : m_declaration_line(line)
{
}

AstNode::~AstNode()
{

}

void AstNode::AddChild(Ast_Node node)
{
  node->m_parent = this;
  m_children.push_back(std::move(node));
}

void AstNode::RemoveChild(Ast_Node node)
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

void AstNode::ReplaceChild(Ast_Node old_node, Ast_Node new_node)
{
  for (size_t i = 0; i < m_children.size(); ++i)
  {
    if (m_children[i] == old_node)
    {
      new_node->m_parent = this;
      m_children[i] = new_node;
      return;
    }
  }
}

AstNode *AstNode::Parent() const
{
  return m_parent;
}

Ast_Node AstNode::GetChildByRawPointer(AstNode *node)
{
  for (size_t i = 0; i < m_children.size(); ++i)
  {
    if (m_children[i].get() == node)
    {
      return m_children[i];
    }
  }

  return nullptr;
}

std::vector<Ast_Node> AstNode::Children() const
{
  return m_children;
}

int AstNode::DeclarationLine() const
{
  return m_declaration_line;
}
