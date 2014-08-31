#include "AstNode.h"

AstNode::AstNode(int line) : m_declaration_line(line)
{
}

AstNode::~AstNode()
{

}

void AstNode::AddChild(AstPtr node)
{
  node->m_parent = this;
  m_children.push_back(std::move(node));
}

void AstNode::AddChildToLeft(AstPtr node)
{
  node->m_parent = this;
  m_children.push_front(node);
}

void AstNode::RemoveChild(AstPtr node)
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

void AstNode::ReplaceChild(AstPtr old_node, AstPtr new_node)
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

AstPtr AstNode::GetChildByRawPointer(AstNode *node)
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

std::deque<AstPtr> AstNode::Children() const
{
  return m_children;
}

int AstNode::DeclarationLine() const
{
  return m_declaration_line;
}
