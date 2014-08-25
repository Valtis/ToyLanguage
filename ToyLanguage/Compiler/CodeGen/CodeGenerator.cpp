#include "CodeGenerator.h"
#include "../../VM/ByteCode.h"


std::unordered_map<int, VMFunction> CodeGenerator::GenerateCode(const std::unordered_map<std::string, Function> functions)
{
  std::unordered_map<std::string, int> m_function_names_to_ids;

  std::unordered_map< int, VMFunction > vm_functions;
  m_function_names_to_ids["main"] = MAIN_FUNCTION_ID;

  int current_id = 1;
  for (auto pair : functions)
  {
    Function f = pair.second;
    if (pair.first == "main")
    {
      vm_functions[MAIN_FUNCTION_ID] = GenerateCodeForFunction(f);
      vm_functions[MAIN_FUNCTION_ID].AddByteCode(ByteCode{Instruction::STOP, nullptr});
    }
    else
    {
      m_function_names_to_ids[pair.first] = current_id;
      vm_functions[current_id] = GenerateCodeForFunction(f);

      ++current_id;
    }
  }
  return vm_functions;
}

VMFunction CodeGenerator::GenerateCodeForFunction(Function &f)
{
  VMFunction vm_function;

  TraverseAst(f.RootNode(), vm_function);
  
  return vm_function;
}


void CodeGenerator::TraverseAst(const Ast_Node &node, VMFunction &vm_function)
{
  if (node == nullptr)
  {
    return;
  }

  for (auto node : node->Children())
  {
    TraverseAst(node, vm_function);
  }


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

  // TODO: replace with something slightly more sane
  if (node->ValueAsText() == "+")
  {
    AddArithmeticInstruction(node->Children().size(), vm_function, Instruction::ADD);
  } 
  else if (node->ValueAsText() == "-")
  {
    AddArithmeticInstruction(node->Children().size(), vm_function, Instruction::SUB);
  }
  else if (node->ValueAsText() == "*")
  {
    vm_function.AddByteCode(ByteCode{ Instruction::MUL, nullptr });
  }
  else if (node->ValueAsText() == "/")
  {
    vm_function.AddByteCode(ByteCode{ Instruction::DIV, nullptr });
  }
  else if (node->ValueAsText() == "print")
  {
    vm_function.AddByteCode(ByteCode{ Instruction::PRINT, nullptr });
  }

}

void CodeGenerator::AddArithmeticInstruction(size_t instruction_child_count, VMFunction & vm_function, Instruction instruction)
{
    vm_function.AddByteCode(ByteCode{ instruction, nullptr });
}