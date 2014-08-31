#include "CodeGenAstVisitor.h"

#include "../FunctionDefines.h"

#include "../DataStructures/AstNodes/NumberNode.h"
#include "../DataStructures/AstNodes/VariableReadNode.h"
#include "../DataStructures/AstNodes/FunctionCallNode.h"
#include "../DataStructures/AstNodes/RootNode.h"


#include "../../VM/VMFunction.h"
#include "../../VM/ByteCode.h"
#include "../../VM/VMObject.h"


#define IF_START 1
#define JUMP_TO_ELSE 2
#define JUMP_TO_END 3

CodeGenAstVisitor::CodeGenAstVisitor(VMFunction *f,
  std::unordered_map<std::string, int> function_names_to_ids)
  : m_function(f), m_function_names_to_ids(function_names_to_ids)
{

  std::unordered_map<std::string, Instruction> inbuilt_functions = { { FN_ADD, Instruction::ADD },
  { FN_SUB, Instruction::SUB }, { FN_MUL, Instruction::MUL }, { FN_DIV, Instruction::DIV },
  { FN_PRINT, Instruction::PRINT }, { FN_PRINTLN, Instruction::PRINTLINE }, { FN_COMPARE, Instruction::COMPARE } };

  // workaround for compiler error C2593 
  m_inbuilt_functions = inbuilt_functions;
}

void CodeGenAstVisitor::Visit(RootNode *node)
{
  if (m_function->Name() == "main")
  {
    m_function->AddByteCode(ByteCode{ Instruction::STOP });
  }
  else
  {
    m_function->AddByteCode(ByteCode{ Instruction::RETURN });
  }

}

void CodeGenAstVisitor::Visit(NumberNode *node)
{
  VMObject o;

  o.type = VMObjectType::NUMBER;
  o.value.number = node->Value();
  m_function->AddByteCode(ByteCode{ Instruction::PUSH, o });
}

void CodeGenAstVisitor::Visit(VariableReadNode *node)
{
  VMObject o;
  o.type = VMObjectType::INTEGER;
  o.value.integer = node->VariableId();
  m_function->AddByteCode(ByteCode{ Instruction::PUSH_VARIABLE, o });
}


void CodeGenAstVisitor::Visit(FunctionCallNode *node)
{

  if (node->Name() == FN_IF)
  {
    GenerateJumpsForIf(node);
    return;
  }

  if (m_inbuilt_functions.count(node->Name()))
  {
    m_function->AddByteCode(ByteCode{ m_inbuilt_functions[node->Name()] });
  }
  else
  {
    int id = m_function_names_to_ids[node->Name()];
    VMObject o;

    o.type = VMObjectType::INTEGER;
    o.value.integer = id;

    m_function->AddByteCode(ByteCode{ Instruction::PUSH, o });
    m_function->AddByteCode(ByteCode{ Instruction::CALLFUNCTION });
  }
}

void CodeGenAstVisitor::GenerateJumpsForIf(FunctionCallNode *node)
{
  auto code = m_function->GetByteCode();


  int else_start_position;
  for (int pos = code.size() - 1; pos >= 0; --pos)
  {
    if (code[pos].GetInstruction() == Instruction::NOP && code[pos].GetObject().type == VMObjectType::INTEGER)
    {
      switch (code[pos].GetObject().value.integer)
      {

        // replace this instruction with unconditional jump to end. Also update the else start position to the instrution after this one
      case JUMP_TO_END:
      {
        VMObject o;
        o.type = VMObjectType::INTEGER;
        o.value.integer = code.size();
        code[pos].ReplaceInstruction(Instruction::JUMP, o);
        else_start_position = pos + 1;
      }
        break;

        // replace this instruction with conditional jump to else branch
      case JUMP_TO_ELSE:
      {
        VMObject o;
        o.type = VMObjectType::INTEGER;
        o.value.integer = else_start_position;
        code[pos].ReplaceInstruction(Instruction::JUMP_IF_FALSE, o);
      }
        break;

      default:
        break;
      }

    }
  }

  m_function->SetByteCode(code);

}

void CodeGenAstVisitor::Visit(IfBooleanNode *node)
{
  VMObject o;
  o.type = VMObjectType::INTEGER;
  o.value.integer = JUMP_TO_ELSE;
  m_function->AddByteCode(ByteCode{ Instruction::NOP, o });
}

void CodeGenAstVisitor::Visit(IfThenNode *node)
{
  VMObject o;
  o.type = VMObjectType::INTEGER;
  o.value.integer = JUMP_TO_END;
  m_function->AddByteCode(ByteCode{ Instruction::NOP, o });
}
