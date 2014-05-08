#include "Function.h"


Function::Function(std::string name, int line) : m_name(name), m_declaration_line(line), m_root_node(nullptr)
{

}


Function::Function() 
{
}


Function::~Function()
{
}

void Function::SetRootNode(std::shared_ptr<AbstractSyntaxTreeNode> node)
{
  m_root_node = node;
}
