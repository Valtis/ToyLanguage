#include "AstNode.h"

AstNode::AstNode(NodeType type) : m_type(type), m_release_char(false)
{
}

AstNode::~AstNode()
{
  MaybeReleaseMemory();
}

void AstNode::MaybeReleaseMemory()
{
  if (m_release_char)
  {
    delete[] m_value.text;
    m_value.text = nullptr;
    m_release_char = false;
  }
}