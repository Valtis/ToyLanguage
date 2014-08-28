#pragma once
#include <cstdint>
#include "VMObject.h"

enum class Instruction : std::uint8_t { NOP, ADD, SUB, MUL, DIV, STOP, PUSH, PUSH_VARIABLE, POP, CALLFUNCTION, RETURN, PRINT, PRINTLINE };




class ByteCode
{
public:
  ByteCode() : instruction(Instruction::NOP), object(nullptr)
  {

  }

  ByteCode(Instruction i, VMObject *o) : instruction(i), object(o)
  {

  }

  ByteCode(const ByteCode &lhs)
  {
    DoCopy(lhs);
  }

  ByteCode &operator=(const ByteCode &lhs)
  {
    if (this != &lhs)
    {
      DoCopy(lhs);
    }
    return *this;
  }

  ~ByteCode()
  {
    delete object;
  }

  Instruction GetInstruction()
  {
    return instruction;
  }

  VMObject *GetObject()
  {
    return object;
  }

  
private:
  void DoCopy(const ByteCode &lhs)
  {
    instruction = lhs.instruction;

    if (lhs.object != nullptr)
    {
      object = new VMObject;
      *object = *lhs.object;
    }
    else
    {
      object = nullptr;
    }
  }

  Instruction instruction;
  VMObject *object; // optional, used by some instructions

};