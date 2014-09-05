#include "CodeGenAstVisitor.h"

#include "../FunctionDefines.h"

#include "../DataStructures/AstNodes/NumberNode.h"
#include "../DataStructures/AstNodes/VariableReadNode.h"
#include "../DataStructures/AstNodes/FunctionCallNode.h"
#include "../DataStructures/AstNodes/FunctionParameterNode.h"
#include "../DataStructures/AstNodes/RootNode.h"


#include "../../VM/VMFunction.h"
#include "../../VM/ByteCode.h"
#include "../../VM/VMObject.h"


#define IF_START 1
#define JUMP_TO_ELSE 2
#define JUMP_TO_END 3

CodeGenAstVisitor::CodeGenAstVisitor(VMFunction *f,
  std::unordered_map<std::string, Function> functions)
  : m_function(f), m_functions(functions)
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

void CodeGenAstVisitor::Visit(FunctionParameterNode *node)
{
  VMObject o;

  o.type = VMObjectType::INTEGER;
  o.value.integer = m_functions[node->Name()].FunctionId();
  m_function->AddByteCode(ByteCode{ Instruction::PUSH, o });
}


void CodeGenAstVisitor::Visit(VariableReadNode *node)
{
  VMObject o;
  o.type = VMObjectType::INTEGER;
  o.value.integer = node->VariableId();
  m_function->AddByteCode(ByteCode{ Instruction::PUSH_VARIABLE, o });
}

// TODO: Extract inbuilt function handling into separate classes instead of creating massive if/else-monstrosity here
// map<std::string(name), FunctionCodeGenerator(inbuild_type)>
// can move arithmetic function transformation from semantic analyzer into generator class
void CodeGenAstVisitor::Visit(FunctionCallNode *node)
{

  if (node->Name() == FN_IF)
  {
    GenerateJumpsForIf(node);
  } 
  else if (node->Name() == FN_LIST)
  {
    CreateList(node);
  }
  else if (node->Name() == FN_MAP)
  {
    CreateMapping(node);
  }
  else if (m_inbuilt_functions.count(node->Name()))
  {
    m_function->AddByteCode(ByteCode{ m_inbuilt_functions[node->Name()] });
  }
  else
  {
    int id = m_functions[node->Name()].FunctionId();
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


void CodeGenAstVisitor::CreateList(FunctionCallNode *node)
{
  size_t child_count = node->Children().size();
  VMObject o;

  o.type = VMObjectType::INTEGER;
  o.value.integer = 0;
  
  // allocate nullptr
  m_function->AddByteCode(ByteCode{ Instruction::PUSH, o });
  m_function->AddByteCode(ByteCode{ Instruction::ALLOCATE_PTR });

  for (size_t i = 0; i < child_count; ++i)
  {
    // allocate space for 2 VMObjects (pointer to next, value)
    o.value.integer = 2;
    m_function->AddByteCode(ByteCode{ Instruction::PUSH, o });
    m_function->AddByteCode(ByteCode{ Instruction::ALLOCATE_PTR });


    // write the next_ptr field
    o.value.integer = 0;

    m_function->AddByteCode(ByteCode{ Instruction::PUSH, o });
    m_function->AddByteCode(ByteCode{ Instruction::WRITE_PTR });

    // write the value field
    o.value.integer = 1;

    m_function->AddByteCode(ByteCode{ Instruction::PUSH, o });
    m_function->AddByteCode(ByteCode{ Instruction::WRITE_PTR });
  }
}

void CodeGenAstVisitor::CreateMapping(FunctionCallNode *node)
{

}