#include "AstNode.h"


AstNode::AstNode() : m_type(OperationType::BLOCK)
{
}

AstNode::AstNode(OperationType type) : m_type(type)
{
}

AstNode::~AstNode()
{
}
