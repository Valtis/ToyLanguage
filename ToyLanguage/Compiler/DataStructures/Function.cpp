#include "Function.h"


Function::Function() : m_name(""), m_declaration_line(0), m_root_node(nullptr)
{

}

Function::Function(std::string name, int line) : m_name(name), m_declaration_line(line), m_root_node(nullptr)
{

}

Function::~Function()
{
}

void Function::SetRootNode(std::shared_ptr<AstNode> node)
{
  m_root_node = node;
}
