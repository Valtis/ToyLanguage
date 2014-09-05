#include "Function.h"


Function::Function() : Function("", 0)
{

}

Function::Function(std::string name, int line) : m_name(name), m_declaration_line(line), m_function_id(-1), m_root_node(nullptr)
{

}

Function::~Function()
{
}

void Function::SetRootNode(std::shared_ptr<AstNode> node)
{
  m_root_node = node;
}
