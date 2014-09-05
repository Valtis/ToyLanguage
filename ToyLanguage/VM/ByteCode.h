#pragma once
#include <cstdint>
#include "VMObject.h"

enum class Instruction : std::uint8_t { NOP, ADD, SUB, MUL, DIV, STOP, PUSH, PUSH_VARIABLE, POP, CALLFUNCTION, RETURN,
  PRINT, PRINTLINE, JUMP_IF_TRUE, JUMP_IF_FALSE, JUMP, COMPARE, ALLOCATE_PTR, READ_PTR, WRITE_PTR };

class ByteCode
{
public:
  ByteCode() : instruction(Instruction::NOP)
  {
    object.type = VMObjectType::NONE;
  }

  ByteCode(Instruction i) : instruction(i)
  {
    object.type = VMObjectType::NONE;
  }

  ByteCode(Instruction i, VMObject o) : instruction(i), object(o)
  {

  }

  void ReplaceInstruction(Instruction i)
  {
    instruction = i;
    object.type = VMObjectType::NONE;
  }

  void ReplaceInstruction(Instruction i, const VMObject &o)
  {
    instruction = i;
    object = o;
  }

  Instruction GetInstruction()
  {
    return instruction;
  }

  VMObject GetObject()
  {
    return object;
  }
    
private:

  Instruction instruction;
  VMObject object; // optional, used by some instructions

};