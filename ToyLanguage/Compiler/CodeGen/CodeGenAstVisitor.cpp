#include "CodeGenAstVisitor.h"

#include "../FunctionDefines.h"

#include "../DataStructures/AstNodes/NumberNode.h"
#include "../DataStructures/AstNodes/VariableReadNode.h"
#include "../DataStructures/AstNodes/FunctionCallNode.h"
#include "../DataStructures/AstNodes/RootNode.h"


#include "../../VM/VMFunction.h"
#include "../../VM/ByteCode.h"
#include "../../VM/VMObject.h"

CodeGenAstVisitor::CodeGenAstVisitor(VMFunction *f, 
  std::unordered_map<std::string, int> function_names_to_ids) 
  : m_function(f), m_function_names_to_ids(function_names_to_ids)
{
  
  std::unordered_map<std::string, Instruction> inbuilt_functions = { { FN_ADD, Instruction::ADD }, 
  { FN_SUB, Instruction::SUB },  { FN_MUL, Instruction::MUL }, { FN_DIV, Instruction::DIV }, 
  { FN_PRINT, Instruction::PRINT }, { FN_PRINTLN, Instruction::PRINTLINE } };

  // workaround for compiler error C2593 
  m_inbuilt_functions = inbuilt_functions;
}

void CodeGenAstVisitor::visit(RootNode *node)
{
  if (m_function->Name() == "main")
  {
    m_function->AddByteCode(ByteCode{ Instruction::STOP, nullptr });
  }
  else
  {
    m_function->AddByteCode(ByteCode{ Instruction::RETURN, nullptr });
  }

}

void CodeGenAstVisitor::visit(NumberNode *node)
{
  VMObject *o = new VMObject;
  
  o->type = VMObjectType::NUMBER;
  o->value.number = node->Value();
  m_function->AddByteCode(ByteCode{ Instruction::PUSH, o });
}

void CodeGenAstVisitor::visit(VariableReadNode *node)
{
  auto o = new VMObject;
  o->type = VMObjectType::INTEGER;
  o->value.integer = node->VariableId();
  m_function->AddByteCode(ByteCode{ Instruction::PUSH_VARIABLE, o });
}


void CodeGenAstVisitor::visit(FunctionCallNode *node)
{

  if (m_inbuilt_functions.count(node->Name()))
  {
    m_function->AddByteCode(ByteCode{ m_inbuilt_functions[node->Name()], nullptr });
  }
  else
  {
    int id = m_function_names_to_ids[node->Name()];
    auto o = new VMObject;
    
    o->type = VMObjectType::INTEGER;
    o->value.integer = id;

    m_function->AddByteCode(ByteCode{ Instruction::PUSH, o });
    m_function->AddByteCode(ByteCode{ Instruction::CALLFUNCTION, nullptr });
  }
}