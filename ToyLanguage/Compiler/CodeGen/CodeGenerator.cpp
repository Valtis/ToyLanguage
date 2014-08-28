#include "CodeGenerator.h"
#include "../../VM/ByteCode.h"
#include "../../Utility/AstFunctions.h"
#include "../FunctionDefines.h"

std::unordered_map<int, VMFunction> CodeGenerator::GenerateCode(const std::unordered_map<std::string, Function> functions)
{


  std::unordered_map< int, VMFunction > vm_functions;
  m_function_names_to_ids["main"] = MAIN_FUNCTION_ID;

  int current_id = 1;
  for (auto pair : functions)
  {
    Function f = pair.second;
    if (pair.first == "main")
    {
      vm_functions[MAIN_FUNCTION_ID] = GenerateCodeForFunction(f);
      vm_functions[MAIN_FUNCTION_ID].AddByteCode(ByteCode{ Instruction::STOP, nullptr });
    }
    else
    {
      m_function_names_to_ids[pair.first] = current_id;
      vm_functions[current_id] = GenerateCodeForFunction(f);

      vm_functions[current_id].AddByteCode(ByteCode{ Instruction::RETURN, nullptr });
      ++current_id;
    }
  }
  return vm_functions;
}

VMFunction CodeGenerator::GenerateCodeForFunction(Function &f)
{
  VMFunction vm_function;

  TraverseAst(f.RootNode(), [&vm_function, this](Ast_Node &node){ this->Generator(node, vm_function); });

  return vm_function;
}


void CodeGenerator::Generator(const Ast_Node &node, VMFunction &vm_function)
{
  switch (node->Type())
  {
  case NodeType::NUMBER:
    GeneratePushNumberInstruction(node, vm_function);
    break;

  case NodeType::FUNCTION_CALL:
    GenerateFunctionCallInstruction(node, vm_function);
    break;

  case NodeType::ROOT:
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

