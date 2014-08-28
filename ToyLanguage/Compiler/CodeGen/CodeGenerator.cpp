#include "CodeGenerator.h"
#include "../../VM/ByteCode.h"
#include "../../Utility/AstFunctions.h"
#include "../FunctionDefines.h"

CodeGenerator::CodeGenerator(const std::unordered_map<std::string, Function> functions, 
  std::unordered_map<std::string, int> function_name_ids) : m_functions(functions), m_function_names_to_ids(function_name_ids)
{

}

std::unordered_map<int, VMFunction> CodeGenerator::GenerateCode()
{

  std::unordered_map< int, VMFunction > vm_functions;
  
  for (auto pair : m_functions)
  {
    Function f = pair.second;
    vm_functions[m_function_names_to_ids[pair.first]] = GenerateCodeForFunction(pair.first, f);
  }
  return vm_functions;
}

VMFunction CodeGenerator::GenerateCodeForFunction(const std::string &current_function_name, Function &f)
{
  VMFunction vm_function(f.ParameterCount());

  TraverseAst(f.RootNode(), [&vm_function, &current_function_name, this](Ast_Node &node){ this->Generator(node, vm_function, current_function_name); });

  return vm_function;
}


void CodeGenerator::Generator(const Ast_Node &node, VMFunction &vm_function, const std::string &current_function_name)
{
  switch (node->Type())
  {
  case NodeType::NUMBER:
    GeneratePushNumberInstruction(node, vm_function);
    break;

  case NodeType::FUNCTION_CALL:
    GenerateFunctionCallInstruction(node, vm_function);
    break;

  case NodeType::VARIABLE:
    GenerateVariableReadInstruction(node, vm_function);
    break;

  case NodeType::ROOT:
    GenerateReturnOrStopInstruction(current_function_name, vm_function);
  default:
    break;
  }
}

void CodeGenerator::GeneratePushNumberInstruction(const Ast_Node & node, VMFunction & vm_function)
{
  VMObject *o = new VMObject;
  o->type = VMObjectType::NUMBER;
  o->value.number = node->ValueAsNumber();

  vm_function.AddByteCode(ByteCode{ Instruction::PUSH, o });

}

void CodeGenerator::GenerateFunctionCallInstruction(const Ast_Node & node, VMFunction & vm_function)
{

  std::unordered_map<std::string, Instruction> inbuilt_functions = { { FN_ADD, Instruction::ADD }, { FN_SUB, Instruction::SUB },
  { FN_MUL, Instruction::MUL }, { FN_DIV, Instruction::DIV }, { FN_PRINT, Instruction::PRINT }, { FN_PRINTLN, Instruction::PRINTLINE } };

  if (inbuilt_functions.count(node->ValueAsText()) != 0)
  {
    vm_function.AddByteCode(ByteCode{ inbuilt_functions[node->ValueAsText()], nullptr });
  }
  else 
  {
    int id = m_function_names_to_ids[node->ValueAsText()];
    auto o = new VMObject;
    o->type = VMObjectType::INTEGER;
    o->value.integer = id;
    vm_function.AddByteCode(ByteCode{ Instruction::PUSH, o });
    vm_function.AddByteCode(ByteCode{ Instruction::CALLFUNCTION, nullptr });
  }

}


void CodeGenerator::GenerateVariableReadInstruction(const Ast_Node & node, VMFunction & vm_function)
{
  auto o = new VMObject;
  o->type = VMObjectType::INTEGER;
  o->value.integer = node->ValueAsInteger();
  vm_function.AddByteCode(ByteCode{ Instruction::PUSH_VARIABLE, o });
}



void CodeGenerator::GenerateReturnOrStopInstruction(const std::string &current_function_name, VMFunction & vm_function)
{
  if (current_function_name == "main")
  {
    vm_function.AddByteCode(ByteCode{ Instruction::STOP, nullptr });
  } 
  else
  {
    vm_function.AddByteCode(ByteCode{ Instruction::RETURN, nullptr });
  }
}
